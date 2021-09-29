#ifndef WIPER_H
#define WIPER_H

#include <QObject>
#include <QRunnable>
#include <QSqlDatabase>
#include <QSqlRecord>
#include <QSqlQuery>
#include <QTextStream>
#include <QDateTime>
#include <QSqlError>




class wiper : public QObject, public QRunnable
{
    Q_OBJECT
public:
    wiper(QSqlDatabase * _conn, int _thread_cnt, QSqlRecord *_station, QObject *parent = 0, bool _verbose = false);
    ~wiper();

protected:
    void run();

signals:

public slots:
    void finishLoading(bool);

private:
    void init();

    bool m_verbose;
    QDateTime _start;
    QSqlDatabase * m_conn;
    QSqlRecord *rs_station;
};

#endif // WIPER_H
