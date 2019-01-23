#ifndef REPLICATIONAPISERVICEIMPL_H
#define REPLICATIONAPISERVICEIMPL_H

#include "soapReplicationApiServiceService.h"
#include<QString>

class ReplicationApiServiceImpl : public ReplicationApiServiceService
{
public:
   // int ip; //client's ip address

    ReplicationApiServiceImpl();
    ReplicationApiServiceImpl(const ReplicationApiServiceImpl& other);
    ReplicationApiServiceImpl* copy();

    int GetStations (struct soap* soap, const std::wstring &login, const std::wstring &password, ns__StationInfo &result);
    int GetStationsResp (struct soap *soap, const std::wstring &login, const std::wstring &password, ns__GetStationsResponse &result);
};

#endif // REPLICATIONAPISERVICEIMPL_H
