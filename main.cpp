#include <QCoreApplication>


#include "soapservice.h"
#include "replicationapiserviceimpl.h"

//int port = 8080;

int main( int argc, char* argv[] )
{
    QCoreApplication app( argc, argv );
 const struct soap _soap;
    QSharedPointer< ReplicationApiServiceImpl > service( new ReplicationApiServiceImpl() );

    //service->soap(&_soap);
    service->bind_flags = SO_REUSEPORT;
    service->send_timeout = service->recv_timeout = 5; // 5 sec socket idle timeout
    service->transfer_timeout = 30;                         // 30 sec message transfer timeout
    SOAP_SOCKET m = service->bind( "0.0.0.0", 8080, 100 );
    if( !soap_valid_socket( m ) ) {
        return 1;
    };
   // service->copy();

    SOAPService::Controller< ReplicationApiServiceImpl > controller( service );

    if( !controller.start() ) {
        return 2;
    }

    return app.exec();
}

