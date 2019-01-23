//gsoap ns service name: ReplicationApiService
//gsoap ns service namespace: urn:ReplicationApiService
//gsoap ns service protocol:        SOAP
//gsoap ns service style: rpc
//gsoap ns service encoding: encoded

//gsoap ns schema namespace:        urn:ReplicationApiService

#include <vector>

template <class T> class std::vector;

typedef std::wstring xsd__string;

enum ns__ErrorCode {
    ns__NO_ERROR_AUTHORIZATION,
    ns__AUTHORIZATION_ERROR_IS
};

enum ns__UserAccessRightCode{
    NONE,
    VIEW,
    EDIT,
    DELETE
};

class ns__StationInfo{
public:
    xsd__string ID;
    int Code;
    xsd__string Name;
    int UpdatePeriod;
    enum ns__UserAccessRightCode UserAccessRight;


};

class ns__ArrayOfStationInfo{
public:
    std::vector<ns__StationInfo> StationsInfo;



};

class ns__GetStationsResponse  {
public:
    ns__ArrayOfStationInfo* GetStationsResult;
    enum ns__ErrorCode ErrorCode;
    struct soap *soap;

};

class ns__StInfo{
public:
    xsd__string ID;
    int Code;
    xsd__string Name;
    int UpdatePeriod;
    enum ns__UserAccessRightCode UserAccessRight;
};

int ns__GetStations (struct soap *soap, xsd__string login, xsd__string password, ns__StationInfo &result);
int ns__GetStationsResp (struct soap *soap, xsd__string login, xsd__string password, ns__GetStationsResponse &result);
