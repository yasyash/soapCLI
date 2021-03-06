/* soapReplicationApiServiceProxy.h
   Generated by gSOAP 2.8.91 for test.h

gSOAP XML Web services tools
Copyright (C) 2000-2018, Robert van Engelen, Genivia Inc. All Rights Reserved.
The soapcpp2 tool and its generated software are released under the GPL.
This program is released under the GPL with the additional exemption that
compiling, linking, and/or using OpenSSL is allowed.
--------------------------------------------------------------------------------
A commercial use license is available from Genivia Inc., contact@genivia.com
--------------------------------------------------------------------------------
*/

#ifndef soapReplicationApiServiceProxy_H
#define soapReplicationApiServiceProxy_H
#include "soapH.h"

    class SOAP_CMAC ReplicationApiServiceProxy {
      public:
        /// Context to manage proxy IO and data
        struct soap *soap;
        /// flag indicating that this context is owned by this proxy and should be deleted by the destructor
        bool soap_own;
        /// Endpoint URL of service 'ReplicationApiServiceProxy' (change as needed)
        const char *soap_endpoint;
        /// Variables globally declared in test.h, if any
        /// Construct a proxy with new managing context
        ReplicationApiServiceProxy();
        /// Copy constructor
        ReplicationApiServiceProxy(const ReplicationApiServiceProxy& rhs);
        /// Construct proxy given a shared managing context
        ReplicationApiServiceProxy(struct soap*);
        /// Construct proxy given a shared managing context and endpoint URL
        ReplicationApiServiceProxy(struct soap*, const char *soap_endpoint_url);
        /// Constructor taking an endpoint URL
        ReplicationApiServiceProxy(const char *soap_endpoint_url);
        /// Constructor taking input and output mode flags for the new managing context
        ReplicationApiServiceProxy(soap_mode iomode);
        /// Constructor taking endpoint URL and input and output mode flags for the new managing context
        ReplicationApiServiceProxy(const char *soap_endpoint_url, soap_mode iomode);
        /// Constructor taking input and output mode flags for the new managing context
        ReplicationApiServiceProxy(soap_mode imode, soap_mode omode);
        /// Destructor deletes deserialized data and its managing context, when the context was allocated by the constructor
        virtual ~ReplicationApiServiceProxy();
        /// Initializer used by constructors
        virtual void ReplicationApiServiceProxy_init(soap_mode imode, soap_mode omode);
        /// Return a copy that has a new managing context with the same engine state
        virtual ReplicationApiServiceProxy *copy();
        /// Copy assignment
        ReplicationApiServiceProxy& operator=(const ReplicationApiServiceProxy&);
        /// Delete all deserialized data (uses soap_destroy() and soap_end())
        virtual void destroy();
        /// Delete all deserialized data and reset to default
        virtual void reset();
        /// Disables and removes SOAP Header from message by setting soap->header = NULL
        virtual void soap_noheader();
        /// Get SOAP Header structure (i.e. soap->header, which is NULL when absent)
        virtual ::SOAP_ENV__Header *soap_header();
        /// Get SOAP Fault structure (i.e. soap->fault, which is NULL when absent)
        virtual ::SOAP_ENV__Fault *soap_fault();
        /// Get SOAP Fault subcode QName string (NULL when absent)
        virtual const char *soap_fault_subcode();
        /// Get SOAP Fault string/reason (NULL when absent)
        virtual const char *soap_fault_string();
        /// Get SOAP Fault detail XML string (NULL when absent)
        virtual const char *soap_fault_detail();
        /// Close connection (normally automatic, except for send_X ops)
        virtual int soap_close_socket();
        /// Force close connection (can kill a thread blocked on IO)
        virtual int soap_force_close_socket();
        /// Print fault
        virtual void soap_print_fault(FILE*);
    #ifndef WITH_LEAN
    #ifndef WITH_COMPAT
        /// Print fault to stream
        virtual void soap_stream_fault(std::ostream&);
    #endif
        /// Write fault to buffer
        virtual char *soap_sprint_fault(char *buf, size_t len);
    #endif
        //
        /// Web service synchronous operation 'GetStations' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetStations(const std::string& login, const std::string& password, struct ns__GetStationsResponse &_param_1) { return this->GetStations(NULL, NULL, login, password, _param_1); }
        /// Web service synchronous operation 'GetStations' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetStations(const char *soap_endpoint_url, const char *soap_action, const std::string& login, const std::string& password, struct ns__GetStationsResponse &_param_1) { return this->send_GetStations(soap_endpoint_url, soap_action, login, password) || this->recv_GetStations(_param_1) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetStations' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetStations(const char *soap_endpoint_url, const char *soap_action, const std::string& login, const std::string& password);
        /// Web service asynchronous operation 'recv_GetStations' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetStations(struct ns__GetStationsResponse &_param_1);
        //
        /// Web service synchronous operation 'GetSensors' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetSensors(const std::string& login, const std::string& password, const std::string& StationID, const std::string& From, const std::string& To, struct ns__GetSensorsResponse &_param_1) { return this->GetSensors(NULL, NULL, login, password, StationID, From, To, _param_1); }
        /// Web service synchronous operation 'GetSensors' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetSensors(const char *soap_endpoint_url, const char *soap_action, const std::string& login, const std::string& password, const std::string& StationID, const std::string& From, const std::string& To, struct ns__GetSensorsResponse &_param_1) { return this->send_GetSensors(soap_endpoint_url, soap_action, login, password, StationID, From, To) || this->recv_GetSensors(_param_1) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetSensors' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetSensors(const char *soap_endpoint_url, const char *soap_action, const std::string& login, const std::string& password, const std::string& StationID, const std::string& From, const std::string& To);
        /// Web service asynchronous operation 'recv_GetSensors' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetSensors(struct ns__GetSensorsResponse &_param_1);
        //
        /// Web service synchronous operation 'GetHistoricalDataBrief' with default endpoint and default SOAP Action header, returns SOAP_OK or error code
        virtual int GetHistoricalDataBrief(const std::string& login, const std::string& password, int AveragePeriod, ns__ArrayOfSensors *sSensors, const std::string& From, const std::string& To, struct ns__GetHistoricalDataBriefResponse &_param_1) { return this->GetHistoricalDataBrief(NULL, NULL, login, password, AveragePeriod, sSensors, From, To, _param_1); }
        /// Web service synchronous operation 'GetHistoricalDataBrief' to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int GetHistoricalDataBrief(const char *soap_endpoint_url, const char *soap_action, const std::string& login, const std::string& password, int AveragePeriod, ns__ArrayOfSensors *sSensors, const std::string& From, const std::string& To, struct ns__GetHistoricalDataBriefResponse &_param_1) { return this->send_GetHistoricalDataBrief(soap_endpoint_url, soap_action, login, password, AveragePeriod, sSensors, From, To) || this->recv_GetHistoricalDataBrief(_param_1) ? this->soap->error : SOAP_OK; }
        /// Web service asynchronous operation 'send_GetHistoricalDataBrief' to send a request message to the specified endpoint and SOAP Action header, returns SOAP_OK or error code
        virtual int send_GetHistoricalDataBrief(const char *soap_endpoint_url, const char *soap_action, const std::string& login, const std::string& password, int AveragePeriod, ns__ArrayOfSensors *sSensors, const std::string& From, const std::string& To);
        /// Web service asynchronous operation 'recv_GetHistoricalDataBrief' to receive a response message from the connected endpoint, returns SOAP_OK or error code
        virtual int recv_GetHistoricalDataBrief(struct ns__GetHistoricalDataBriefResponse &_param_1);
    };
#endif
