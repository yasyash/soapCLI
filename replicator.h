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
    ns__AUTHORIZATION_IS_OK,
    ns__AUTHORIZATION_ERROR_IS,
    ns__CONNECTION_NOT_ESTABLISHED,
    ns__ERROR_QUERY_EXEC,
    ns__ERROR_DB_BUSY
};

enum ns__UserAccessRightCode{
    NONE,
    VIEW,
    EDIT,
    DELETE
};

enum ns__MeasurementClasses{
    ns__MeasurementClasses_data,
    ns__MeasurementClasses_alert,
    ns__MeasurementClasses_hum_out,
    ns__MeasurementClasses_none
};

class ns__StationInfo{
public:
    xsd__string ID;
    int Code;
    xsd__string Name;
    int UpdatePeriod;
    xsd__string Place;
    float Latitude;
    float Longitude;
    xsd__string UserAccessRight;
};

class ns__UnitInfo
{ public:
    xsd__string ID;
    xsd__string Name;
};


class ns__SensorInfo
{
public:
    xsd__string ID;
    xsd__string Name;
    int AveragePeriod;
    ns__UnitInfo* Unit;
    enum ns__MeasurementClasses MeasurClass;
    xsd__string StationID;
    bool IsWeathercock;
    double PDKValue;
    double PDKDayValue;
    int DefaultColor;
};


class ns__ArrayOfStationInfo
{
public:
    std::vector<ns__StationInfo> StationsInfo;
};

class ns__ArrayOfSensorsInfo{
public:
    std::vector<ns__SensorInfo> SensorsInfo;
};

class ns__GetStationsResponse  {
public:
    ns__ArrayOfStationInfo* GetStationsResult;
    enum ns__ErrorCode ErrorCode;
    struct soap *soap;
};

class ns__GetSensorsResponse  {
public:
    ns__ArrayOfSensorsInfo* GetSensorsResult;
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

int ns__GetStations (struct soap *soap, xsd__string login, xsd__string password, ns__GetStationsResponse &result);
int ns__GetSensors (struct soap *soap, xsd__string login, xsd__string password, xsd__string StationID, xsd__string From, xsd__string To, ns__GetSensorsResponse &result);
