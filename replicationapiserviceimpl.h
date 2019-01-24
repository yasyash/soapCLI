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
    ReplicationApiServiceImpl(const ReplicationApiServiceImpl& other);
    ReplicationApiServiceImpl* copy();

    int GetStations (struct soap *soap, const std::wstring &login, const std::wstring &password, ns__GetStationsResponse &result);

private:
    QSqlDatabase conn;

};

#endif // REPLICATIONAPISERVICEIMPL_H
