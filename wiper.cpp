#include "wiper.h"
#include "ReplicationApiService.nsmap"
#include "soapReplicationApiServiceProxy.h"
#include <QMutex>

extern QMutex* dbmutex;

wiper::wiper(QSqlDatabase * _conn, int _thread_cnt, QSqlRecord *_station, QObject *parent, bool _verbose)   :
    QObject(parent)
{
    m_conn = new QSqlDatabase();
    while(!dbmutex->tryLock())
    {};

    _start = QDateTime::currentDateTime();

    //QTextStream(stdout) << " Locked " << _thread_cnt;
    *m_conn = QSqlDatabase::cloneDatabase(*_conn, QString("weather").append(QString::number(_thread_cnt)));

    m_conn->open();
    bool status = m_conn->open();
    if (!status)
    {
        //releaseModbus();

        QTextStream(stdout) << ( QString("Connection " + m_conn->connectionName() + " error: " + m_conn->lastError().text()).toLatin1().constData()) <<   " \n\r";

    } else {

        rs_station = new QSqlRecord(*_station);
        //rs_station = _station;

        init();
    }

    if (_verbose) m_verbose = true;

    dbmutex->unlock();
    //QTextStream(stdout) << " UNLocked " << _thread_cnt;

}

wiper::~wiper()
{
    m_conn->close();
    //m_conn->removeDatabase(m_conn->connectionName());
    delete m_conn;
    QTextStream(stdout) << "\n Thread for station " << rs_station->value("namestation").toString() << "  is ended... \n\n";

    QTextStream(stdout) << "Total calculation time is " << _start.msecsTo(QDateTime::currentDateTime()) << " milliseconds \n\n";
}

void wiper::init()
{

}

void wiper::finishLoading(bool)
{
    QTextStream(stdout) << "END wiping";
}

void wiper::run()
{
    ns__GetHistoricalDataBriefResponse resp;
    ReplicationApiServiceProxy qr;
    ns__ArrayOfSensors *sSensors;
    std::wstring SensorID;

    sSensors = new ns__ArrayOfSensors();

    QSqlQuery *query_eq = new QSqlQuery(*m_conn);
    QSqlRecord rs_sensor;

    QString ip = QString(rs_station->value("address").toString());
    QString login = QString(rs_station->value("login").toString());
    QString pwd = QString(rs_station->value("password_soap").toString());
    QString select_eq =QString( "SELECT * FROM equipments WHERE idd='"+ rs_station->value("idd").toString() +"' AND measure_class <> 'local' AND is_present = true");
    QString to = QDateTime(QDateTime::currentDateTime()).toString("yyyy-MM-ddThh:mm:ss");
    QDateTime _to = QDateTime::fromString(to,"yyyy-MM-ddTHH:mm:ss");
    int _count = 0;
    int _g_count = 0;
    //soap->override_host = ip.toLocal8Bit().data();
    //soap->override_port = 8080;
    QTextStream(stdout) <<"\nStation === "<< rs_station->value("namestation").toString() <<" === UUID === "<< rs_station->value("idd").toString() <<" ===\n";

    query_eq->prepare(select_eq);
    query_eq->exec();
    if(!query_eq->lastError().isValid())
    {
        while (query_eq->next()) {
            rs_sensor =query_eq->record();
            QString from = rs_sensor.value("date_time_out").toString();
            QDateTime _from = QDateTime::fromString(from.left(19),"yyyy-MM-ddTHH:mm:ss");

            qint64 _difference = _from.secsTo(_to);

            sSensors->string.clear();
            sSensors->string.push_back(QString(rs_sensor.value("serialnum").toString()).toStdString());

            QTextStream(stdout) << "\nQuery === for sensor === "<< rs_sensor.value("typemeasure").toString() << " === \n Serial num. ==="<< rs_sensor.value("serialnum").toString() << "\n Time period is from " << from <<" to " << to <<"\n";


            //if (qr.GetHistoricalDataBrief(ip.toLocal8Bit().data(), "",login.toStdString(), pwd.toStdString(), 60, sSensors, from.toStdString(), to.toStdString(), resp ) == SOAP_OK)
            if (qr.GetHistoricalDataBrief(ip.toLocal8Bit().data(), "",login.toStdString(), pwd.toStdString(), 60, sSensors, from.toStdString(), to.toStdString(), resp ) == SOAP_OK)
            {

                std::vector<ns__BriefData *> _sd = resp.GetHistoricalDataBriefResult->SensorData[0]->Data->BriefData;
                std::vector<ns__BriefData *>::iterator _it;
                QMap<QDateTime, double> _data;
                QMap<QDateTime, double>::iterator _it_dt;

                QSqlQuery *query_chk = new QSqlQuery(*m_conn);
                QSqlRecord rs_data_chk;
                QSqlQuery *query_ins_upd = new QSqlQuery(*m_conn);

                for (_it = _sd.begin(); _it != _sd.end(); _it++)
                {
                    //QTextStream(stdout) << "Time - " << QString::fromStdString((*_it.base())->Time);
                    _data.insert(QDateTime::fromString(QString::fromStdString((*_it.base())->Time.substr(0,19)), "yyyy-MM-ddTHH:mm:ss"), (*_it.base())->Value);

                    //QTextStream(stdout) << "Station UUID = " << rs_station->value("idd").toString() << " sensor's UUID " <<  rs_sensor.value("serialnum").toString() << " \n";
                }
                for (_it_dt = _data.begin(); _it_dt != _data.end(); _it_dt++)
                {

                    QString check = QString("SELECT * FROM sensors_data WHERE serialnum='"+ rs_sensor.value("serialnum").toString() +"' AND date_time='"+
                                            _it_dt.key().toString("yyyy-MM-ddTHH:mm:ss")+"'");

                    query_chk->prepare(check);
                    query_chk->exec();

                    if (m_verbose)
                        QTextStream(stdout) << "\nResult === time is === " <<  _it_dt.key().toString("yyyy-MM-ddTHH:mm:ss") << " === and value ==="<<  _it_dt.value() << " === type === " << rs_sensor.value("typemeasure").toString() <<" === UUID sensor === "<< rs_sensor.value("serialnum").toString() << " \n";

                    if (!query_chk->next())
                    {
                        QString insert = "INSERT INTO sensors_data (  idd, serialnum, date_time, typemeasure, measure, is_alert) VALUES (:idd, :serialnum, :date_time, :typemeasure, :measure, :is_alert)";
                        query_ins_upd->prepare(insert);

                        query_ins_upd->bindValue(":idd",rs_station->value("idd").toString());
                        query_ins_upd->bindValue(":serialnum", rs_sensor.value("serialnum").toString());
                        query_ins_upd->bindValue(":date_time",  _it_dt.key().toString("yyyy-MM-ddTHH:mm:ss"));
                        query_ins_upd->bindValue(":typemeasure", rs_sensor.value("typemeasure").toString());
                        query_ins_upd->bindValue(":measure",  _it_dt.value());
                        query_ins_upd->bindValue(":is_alert", false);

                        query_ins_upd->exec();

                        if (!query_ins_upd->lastError().isValid())
                        {
                            query_chk->finish();
                            QString chk_time = "SELECT * FROM equipments WHERE serialnum = :serialnum AND idd = :idd";
                            query_chk->prepare(chk_time);
                            query_chk->bindValue(":idd",rs_station->value("idd").toString());
                            query_chk->bindValue(":serialnum", rs_sensor.value("serialnum").toString());
                            query_chk->exec();
                            query_chk->next();
                            QSqlRecord chk_sensor = query_chk->record();

                            //QString _str = QString::fromStdString((*_it.base())->Time.substr(0,19));
                            //QDateTime _dt = QDateTime::fromString(_str, "yyyy-MM-ddTHH:mm:ss");

                            //QTextStream(stdout) << "TIME is " << chk_sensor.value("date_time_out").toString() << " *** "  <<" *** \n";
                            if (chk_sensor.value("date_time_out").toDateTime() <  _it_dt.key())
                            {
                                query_ins_upd->clear();
                                QString update = "UPDATE equipments SET date_time_out = :date_time_out WHERE serialnum = :serialnum AND idd = :idd";
                                query_ins_upd->prepare(update);
                                query_ins_upd->bindValue(":idd",rs_station->value("idd").toString());
                                query_ins_upd->bindValue(":serialnum", rs_sensor.value("serialnum").toString());
                                query_ins_upd->bindValue(":date_time_out",  _it_dt.key().toString("yyyy-MM-ddTHH:mm:ss"));
                                query_ins_upd->exec();

                                if (!query_ins_upd->lastError().isValid())
                                {
                                    _count++;
                                    _g_count++;
                                } else {
                                    QTextStream(stdout) << "Error update equipment time: " << query_ins_upd->lastError().text() << " \n";
                                }
                            } else {
                                QTextStream(stdout) << chk_sensor.value("date_time_out").toString() << "   "  << "  Time early is detected...\n" <<
                                                       _count++;
                                _g_count++;
                            }
                        }

                    }else {
                        QTextStream(stdout) << "Error! Record is already exist! Go to a next...\n";
                    }
                }

                query_chk->finish();
                //query_ins_upd->first();
                query_ins_upd->finish();
            }

            QTextStream(stdout) << "\n================================================================\n";
            QTextStream(stdout) << "\nThere are " << _count << " records for sensor UUID = "<< rs_sensor.value("serialnum").toString() <<" added from SOAP query\n";
            _count = 0;
        }
    }

    query_eq->finish();


}
