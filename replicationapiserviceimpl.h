#ifndef REPLICATIONAPISERVICEIMPL_H
#define REPLICATIONAPISERVICEIMPL_H

#include "soapReplicationApiServiceService.h"
#include<QString>
#include <QtSql>


class ReplicationApiServiceImpl : public ReplicationApiServiceService
{
public:
    // int ip; //client's ip address

    ReplicationApiServiceImpl();
    ReplicationApiServiceImpl(const ReplicationApiServiceImpl & other);
    ReplicationApiServiceImpl* copy();
    ~ReplicationApiServiceImpl();

    int GetStations (struct soap *soap, const std::wstring &login, const std::wstring &password, ns__GetStationsResponse &result);
    int GetSensors (struct soap *soap, const std::wstring &login, const std::wstring &password, const std::wstring &StationID, const std::wstring &From, const std::wstring &To, ns__GetSensorsResponse &result);
    void addThreadCount(const int &cnt);
private:
    QVector<QSqlDatabase> connV;
    QSqlDatabase *conn;

};

#endif // REPLICATIONAPISERVICEIMPL_H
