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
    { //int j = 1;
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
        result.ErrorCode = authoization_error_;

        return SOAP_OK;
    }

    QSqlQuery query = QSqlQuery(*conn);
    query.prepare("SELECT * FROM stations WHERE is_present = true");
    query.exec();
    if(query.lastError().isValid())
    {
        result.ErrorCode = query_error_;

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

    result.ErrorCode = authoization_ok_;
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
        result.ErrorCode = authoization_error_;
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
        result.ErrorCode = query_error_;
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

        if (recordset.value("measure_class").toString() == "data") si.MeasurClass = data_;
        if (recordset.value("measure_class").toString() == "alert") si.MeasurClass = alert_;
        if (recordset.value("measure_class").toString() == "hum_out") si.MeasurClass = hum_out_;

        si.StationID = StationID;
        si.IsWeathercock = recordset.value("is_wind_sensor").toBool() ? true : false;
        si.PDKValue = recordset.value("max_consentration").toDouble();
        si.PDKDayValue = recordset.value("max_day_consentration").toDouble();
        si.DefaultColor = recordset.value("def_colour").toInt();



        result.GetSensorsResult->SensorsInfo.push_back(si);

    }

    result.ErrorCode = authoization_ok_;
    recordset.clear();
    query->finish();
    query->clear();
    delete query;

    conn->close();

    dbmutex->unlock();
    fprintf(stderr, "GetSensors is done \n");

    return SOAP_OK;


}

int ReplicationApiServiceImpl::GetHistoricalDataBrief (struct soap *soap, const std::wstring &login, const std::wstring &password, int AveragePeriod, const ns__ArrayOfSensors &sSensors, const std::wstring &From, const std::wstring &To, ns__GetHistoricalDataBriefResponse &result)
{
    while(!dbmutex->tryLock());

    ns__BriefData *bd;
    ns__SensorData sd;

    result.GetHistoricalDataBriefResult = new ns__ArrayOfSensorData();
    sd.Data = new ns__ArrayOfBriefData();
    //sd.Data->BriefData = new  std::vector<ns__BriefData>;
    std::vector<ns__BriefData*> abd;

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
        result.ErrorCode = authoization_error_;
        fprintf(stderr, "DB - BUSY \n");
        dbmutex->unlock();

        return SOAP_OK;
    }
    //}
    QSqlQuery *query = new QSqlQuery(*conn);
    std::wstring SensorID;
    QSqlRecord recordset;

    for (std::size_t _i =0; _i < sSensors.string.size(); _i++)
    {


        SensorID = sSensors.string[_i];

        QString str = QString::fromStdWString(L"SELECT * FROM sensors_data WHERE  serialnum = '" + SensorID +L"' AND date_time > '" + From + L"' AND date_time <='" + To + L"' ORDER BY date_time ASC");
        query->prepare(str);
        query->exec();

        if(!query->lastError().isValid())
        {
            while (query->next()) {
                bd = new ns__BriefData();
                recordset = query->record();

                bd->Time = recordset.value("date_time").toString().toStdWString();
                bd->Value = recordset.value("measure").toDouble();

                sd.Data->BriefData.push_back(bd);

                //delete  bd;
            }

            sd.SensorID = SensorID;

            result.GetHistoricalDataBriefResult->SensorData.push_back(sd);
            result.ErrorCode = authoization_ok_;
            recordset.clear();
            query->finish();
            query->clear();
        }
        else
        {

            sd.SensorID = SensorID;

            result.GetHistoricalDataBriefResult->SensorData.push_back(sd);
            result.ErrorCode = query_error_;
            query->finish();
            query->clear();

        }
    }

    delete query;

    conn->close();

    dbmutex->unlock();
    fprintf(stderr, "GetHistoricalDataBrief is done \n");


    return SOAP_OK;
}
