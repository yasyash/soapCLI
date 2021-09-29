#include "global.h"

#include <QTextStream>
#include <QSqlDatabase>
#include <QSqlQuery>
#include <QSqlRecord>
#include <QSqlError>
#include <QSqlField>
#include <QDateTime>
#include <QThreadPool>
#include <QMutex>
#include <QSharedMemory>

#include "wiper.h"

int main( int argc, char* argv[] )
{
    QSharedMemory _shared("77777777-3333-7777-3333-4dafd2077c46");
    if( !_shared.create( 512, QSharedMemory::ReadWrite) )
    {
    QTextStream(stdout) << "Can't start more than one instance of the application...\n\r";
    exit(-1);
    }

    dbmutex = new QMutex(QMutex::NonRecursive);

    QString db ="";
    if (QString(argv[1]).contains(QString("-db")))
        db = QString(argv[2]);
    if (db == "")
    {
        // releaseModbus();

       QTextStream(stdout) << "Fatal error: wrong data of the database parameter\n\r";
        exit(-1);

    }

    QString user = QString(argv[4]);
    if (user == "")
    {
        // releaseModbus();

        QTextStream(stdout) << "Fatal error: wrong data of the user parameter\n\r";
        exit(-1);

    }

    QString pw = QString(argv[6]);
    if (pw == "")
    {
        // releaseModbus();

        QTextStream(stdout) << "Fatal error: wrong data of the password parameter\n\r";
        exit(-1);

    }

    const bool _verb = QString(argv[7]).contains(QString("-v"));

    QThreadPool *threadPool = new QThreadPool();
    threadPool->setMaxThreadCount(20);

    wiper *m_wiper;

    QSqlDatabase * m_conn = new QSqlDatabase();
    *m_conn = QSqlDatabase::addDatabase("QPSQL");
    m_conn->setHostName("localhost");
    m_conn->setDatabaseName(db);
    m_conn->setUserName(user);
    m_conn->setPassword(pw);

    bool status = m_conn->open();
    int db_count = 0;

    if (!status)
    {
        //releaseModbus();

        QTextStream(stdout) << ( QString("Connection error: " + m_conn->lastError().text()).toLatin1().constData()) <<   " \n\r";
        return -1;

    }

    QString select_st = "SELECT * FROM stations WHERE is_present = true";
    QSqlQuery *query = new QSqlQuery(*m_conn);
    QSqlRecord rs_station;

    query->prepare(select_st);
    query->exec();

    if(!query->lastError().isValid())
    {
        while (query->next()) {
            rs_station = query->record();
            db_count++;

            m_wiper = new wiper(m_conn, db_count, &rs_station, nullptr, _verb);

            //connect(m_wiper, SIGNAL(threadFinished(int)), m_wiper, SLOT(finishLoading(int)), Qt::QueuedConnection);


                   threadPool->start(m_wiper);
                    QTextStream(stdout) << "\n***************  start (thread): " << threadPool->activeThreadCount() << " **************\n";


            QTextStream(stdout) << "================================================================\n";

            QTextStream(stdout) << "\nStation name is "<< rs_station.value("namestation").toString() <<" and UUID = " << rs_station.value("idd").toString() << " \n";

            QTextStream(stdout) << "\n================================================================\n";
        }
    }
    query->finish();

    while (threadPool->activeThreadCount());

    delete threadPool;
    m_conn->close();
    _shared.detach();
}

