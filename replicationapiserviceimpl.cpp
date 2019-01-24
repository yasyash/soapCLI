#include "replicationapiserviceimpl.h"
#include "ReplicationApiService.nsmap"

//#include "share.h"
#include <vector>

struct soap;

ReplicationApiServiceImpl::ReplicationApiServiceImpl()
{

}

ReplicationApiServiceImpl::ReplicationApiServiceImpl(const ReplicationApiServiceImpl &other) : ReplicationApiServiceService(other)
{


}



ReplicationApiServiceImpl *ReplicationApiServiceImpl::copy()
{

    return new ReplicationApiServiceImpl (*this);
}





int ReplicationApiServiceImpl::GetStations (struct soap *soap, const std::wstring &login, const std::wstring &password, ns__GetStationsResponse &result)
{
    conn = QSqlDatabase::addDatabase("QPSQL");
    conn.setHostName("127.0.0.1"); //localhost database
    conn.setDatabaseName("weather"); //hardcoded name
    conn.setUserName( QString::fromStdWString(login));
    conn.setPassword(QString::fromStdWString(password));
    bool status = conn.open();

    if (!status)
    {
        result.ErrorCode = ns__AUTHORIZATION_ERROR_IS;

        return SOAP_OK;
    }

    QSqlQuery query = QSqlQuery(conn);
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
    query.finish();
    conn.close();


    return SOAP_OK;
}
