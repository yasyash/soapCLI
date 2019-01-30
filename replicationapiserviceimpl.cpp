#include "replicationapiserviceimpl.h"
#include "ReplicationApiService.nsmap"

#include <vector>

extern QMutex* dbmutex;
extern QWaitCondition* db_execution_wait;
extern QMutexLocker* locker;


struct soap;

ReplicationApiServiceImpl::ReplicationApiServiceImpl()
{
    connV = QVector<QSqlDatabase>();

}

ReplicationApiServiceImpl::ReplicationApiServiceImpl(const ReplicationApiServiceImpl &other) : ReplicationApiServiceService(other)
{
    conn = new  QSqlDatabase();
    while(!dbmutex->tryLock())
    {};
    QStringList _str =  conn->connectionNames();
    if (!_str.contains("weather"))
    {

        *conn = QSqlDatabase::addDatabase("QPSQL", "weather");

    }
    else
    {

        *conn = QSqlDatabase::database("weather");

    }
    dbmutex->unlock();


}

ReplicationApiServiceImpl::~ReplicationApiServiceImpl()
{
    //conn->removeDatabase(conn->connectionName());
    delete conn;
    //delete locker;

}

ReplicationApiServiceImpl *ReplicationApiServiceImpl::copy()
{

    return new ReplicationApiServiceImpl (*this);
}

void ReplicationApiServiceImpl::addThreadCount(const int &cnt)
{
    conn = new  QSqlDatabase();

    QStringList _str =  conn->connectionNames();
    QString i = QString("weather%1").arg(cnt, 0, 10);
    fprintf(stderr, "DB Pool - %d \n", _str.count());

    if (!_str.contains(i))
    {
        *conn = QSqlDatabase::addDatabase("QPSQL", i);
    }
    else
    { int j = 1;
        while(!dbmutex->tryLock() )
        {
        }

        i = QString("weather%1").arg(_str.size()+1, 0, 10);
        *conn = QSqlDatabase::addDatabase("QPSQL", i);
        dbmutex->unlock();
        fprintf(stderr, "DB - %s \n", i.toLatin1().data());

        return;
    }




    fprintf(stderr, "DB - %s \n", i.toLatin1().data());

    //return;
}




int ReplicationApiServiceImpl::GetStations (struct soap *soap, const std::wstring &login, const std::wstring &password, ns__GetStationsResponse &result)
{
    while(!dbmutex->tryLock());

    bool status = conn->isOpen();
    if (!status)
    {
        conn->setHostName("127.0.0.1"); //localhost database
        conn->setDatabaseName("weather"); //hardcoded name
        conn->setUserName( QString::fromStdWString(login));
        conn->setPassword(QString::fromStdWString(password));

        status = conn->open();

    }

    if (!status)
    {
        result.ErrorCode = ns__AUTHORIZATION_ERROR_IS;

        return SOAP_OK;
    }

    QSqlQuery query = QSqlQuery(*conn);
    query.prepare("SELECT * FROM stations WHERE is_present = true");
    query.exec();
    if(query.lastError().isValid())
    {
        result.ErrorCode = ns__ERROR_QUERY_EXEC;

        return SOAP_OK;
    }

    result.GetStationsResult = new ns__ArrayOfStationInfo();
    ns__StationInfo si;
    QSqlRecord recordset;
    query.first();


    do {
        recordset = query.record();
        si.ID = recordset.value("idd").toString().toStdWString();
        si.Code = recordset.value("code").toInt();
        si.Name = recordset.value("namestation").toString().toStdWString();
        si.UpdatePeriod = recordset.value("updateperiod").toInt();
        si.UserAccessRight = recordset.value("useraccessright").toString().toStdWString();
        si.Place = recordset.value("place").toString().toStdWString();
        si.Latitude = recordset.value("latitude").toFloat();
        si.Longitude = recordset.value("longitude").toFloat();
        result.GetStationsResult->StationsInfo.push_back(si);

    } while (query.next());

    result.ErrorCode = ns__AUTHORIZATION_IS_OK;
    recordset.clear();
    query.finish();
    query.clear();
    conn->close();

    dbmutex->unlock();
    fprintf(stderr, "GetStations is done \n");

    return SOAP_OK;
}

int ReplicationApiServiceImpl::GetSensors (struct soap *soap, const std::wstring &login, const std::wstring &password, const std::wstring &StationID, const std::wstring &From, const std::wstring &To, ns__GetSensorsResponse &result)
{

    while(!dbmutex->tryLock());


    bool status = conn->isOpen();
    if (!status)
    {
        conn->setHostName("127.0.0.1"); //localhost database
        conn->setDatabaseName("weather"); //hardcoded name
        conn->setUserName( QString::fromStdWString(login));
        conn->setPassword(QString::fromStdWString(password));

        status = conn->open();

    }

      if (!status)
    {
        result.ErrorCode = ns__AUTHORIZATION_ERROR_IS;
        fprintf(stderr, "DB - BUSY \n");
        dbmutex->unlock();

        return SOAP_OK;
    }
    //}

    QSqlQuery *query = new QSqlQuery(*conn);
    QString str = QString::fromStdWString(L"SELECT * FROM equipments WHERE idd = '" + StationID +L"' AND is_present = true");
    query->prepare(str);
    query->exec();

    if(query->lastError().isValid())
    {
        result.ErrorCode = ns__ERROR_QUERY_EXEC;
        dbmutex->unlock();

        return SOAP_OK;
    }

    result.GetSensorsResult = new ns__ArrayOfSensorsInfo();
    ns__SensorInfo si;
    si.Unit = new ns__UnitInfo();
    QSqlRecord recordset;
    //query.first();


    while (query->next()) {
        recordset = query->record();
        si.ID = recordset.value("serialnum").toString().toStdWString();
        si.Name = recordset.value("typemeasure").toString().toStdWString();
        si.AveragePeriod = recordset.value("updateperiod").toInt();
        si.Unit->Name = recordset.value("unit_name").toString().toStdWString();

        if (recordset.value("measure_class").toString() == "data") si.MeasurClass = ns__MeasurementClasses_data;
        if (recordset.value("measure_class").toString() == "alert") si.MeasurClass = ns__MeasurementClasses_alert;
        if (recordset.value("measure_class").toString() == "hum_out") si.MeasurClass = ns__MeasurementClasses_hum_out;

        si.StationID = StationID;
        si.IsWeathercock = recordset.value("is_wind_sensor").toBool() ? true : false;
        si.PDKValue = recordset.value("max_consentration").toDouble();
        si.PDKDayValue = recordset.value("max_day_consentration").toDouble();
        si.DefaultColor = recordset.value("def_colour").toInt();



        result.GetSensorsResult->SensorsInfo.push_back(si);

    }

    result.ErrorCode = ns__AUTHORIZATION_IS_OK;
    recordset.clear();
    query->finish();
    query->clear();
    delete query;

    conn->close();

    dbmutex->unlock();
    fprintf(stderr, "GetSensors is done \n");

    return SOAP_OK;


}

