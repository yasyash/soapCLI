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
    //    soap  (other);

    //    soap_copy_context(this, &other);
    // this->soap_endpoint = other.soap_endpoint;

}



ReplicationApiServiceImpl *ReplicationApiServiceImpl::copy()
{

    return new ReplicationApiServiceImpl (*this);
}




int ReplicationApiServiceImpl::GetStations (struct soap* soap, const std::wstring &login, const std::wstring &password, ns__StationInfo &result)
{
    const std::wstring user = L"admin";
    const std::wstring pwd = L"31415";


    std::wcout << "username "<< login;

    // result.StationsInfo(0);

    if ((login != user) || (password != pwd)) {
        result.ID = login;
        result.UserAccessRight = NONE;
    }
    else{

        result.ID = L"sdfsdfs-sdfsdf-sdf-dsfsdf-sdf";
        result.Code = 111;
        result.Name = L"OPTEC";
        result.UpdatePeriod = 60;
        result.UserAccessRight = VIEW;

    }

    return SOAP_OK;
}

int ReplicationApiServiceImpl::GetStationsResp (struct soap *soap, const std::wstring &login, const std::wstring &password, ns__GetStationsResponse &result)
{
    const std::wstring user = L"admin";
    const std::wstring pwd = L"31415";
    ns__StationInfo si;
   // std::vector<ns__StationInfo> vsi;
    //    result.soap = soap;

    std::wcout << "username "<< login;

    // result.StationsInfo(0);

    if ((login != user) || (password != pwd)) {
        //        si.ID = login;
        // result.StationsInfo.push_back(&si);
        result.ErrorCode = ns__AUTHORIZATION_ERROR_IS;
    }
    else{

                si.ID = L"11153d5c-496a-49bf-935b-e1dbca815111";
                si.Code = 111;
                si.Name = L"OPTEC-T";
                si.UpdatePeriod = 60;
                si.UserAccessRight = VIEW;
        //(std::vector<ns__StationInfo>) result.GetStationsResult->StationsInfo = std::vector<ns__StationInfo>(vsi);
        //vsi.push_back(si);
         result.GetStationsResult = new ns__ArrayOfStationInfo();
         result.GetStationsResult->StationsInfo.push_back(si);

         si.ID = L"urrrrrraaa-a-asdasda-a-sd-as-das-d";
         si.Code = 132424534;
         si.Name = L"OPTEC111333";
         si.UpdatePeriod = 10;
         si.UserAccessRight = EDIT;
         result.GetStationsResult->StationsInfo.push_back(si);

        //vsi.swap(  result.GetStationsResult->StationsInfo);
        result.ErrorCode = ns__NO_ERROR_AUTHORIZATION;
    }

    return SOAP_OK;
}
