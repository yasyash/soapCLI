//gsoap ns service name: ReplicationApiService
//gsoap ns service namespace: urn:ReplicationApiService
//gsoap ns service protocol: SOAP
//gsoap ns service style: rpc
//gsoap ns service encoding: encoded

//gsoap ns schema namespace: urn:ReplicationApiService

#include <vector>

template <class T> class std::vector;

typedef std::wstring xsd__string;

enum ns__ErrorCode {
    authoization_ok_,
    authoization_error_,
    connection_error_,
    query_error_,
    db_busy_
};

enum ns__UserAccessRightCode{
    none_,
    view_,
    edit_,
    delete_
};

enum ns__MeasurementClasses{
    data_,
    alert_,
    hum_out_,
    nothing_
};

class ns__BriefData
{ public:
    xsd__string Time;
    double Value;
};

class ns__ArrayOfBriefData
{ public:
    std::vector<ns__BriefData*> BriefData;
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

class ns__SensorData
{
public:
    xsd__string SensorID;
    ns__ArrayOfBriefData* Data;
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

class ns__ArrayOfSensorData{
public:
    std::vector<ns__SensorData> SensorData;
};

class ns__GetStationsResponse {
public:
    ns__ArrayOfStationInfo* GetStationsResult;
    enum ns__ErrorCode ErrorCode;
    struct soap *soap;
};

class ns__GetSensorsResponse {
public:
    ns__ArrayOfSensorsInfo* GetSensorsResult;
    enum ns__ErrorCode ErrorCode;
    struct soap *soap;
};

class ns__GetHistoricalDataBriefResponse {
public:
    ns__ArrayOfSensorData* GetHistoricalDataBriefResult;
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

class ns__ArrayOfSensors
{ public:
    std::vector<xsd__string> string;
};

int ns__GetStations (struct soap *soap, xsd__string login, xsd__string password, ns__GetStationsResponse &result);
int ns__GetSensors (struct soap *soap, xsd__string login, xsd__string password, xsd__string StationID, xsd__string From, xsd__string To, ns__GetSensorsResponse &result);
int ns__GetHistoricalDataBrief (struct soap *soap, xsd__string login, xsd__string password, int AveragePeriod, ns__ArrayOfSensors sSensors, xsd__string From, xsd__string To, ns__GetHistoricalDataBriefResponse &result);
