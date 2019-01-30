#include <QCoreApplication>

#include "global.h"
#include "soapservice.h"
#include "replicationapiserviceimpl.h"


int main( int argc, char* argv[] )
{
    dbmutex = new QMutex(QMutex::NonRecursive);
    db_execution_wait = new QWaitCondition();

    QCoreApplication app( argc, argv );
    QSharedPointer< ReplicationApiServiceImpl > service( new ReplicationApiServiceImpl() );

    service->bind_flags = SO_REUSEPORT;
    service->send_timeout = service->recv_timeout = 5; // 5 sec socket idle timeout
    service->transfer_timeout = 30;                         // 30 sec message transfer timeout
    SOAP_SOCKET m = service->bind( "0.0.0.0", 8080, 100 );
    if( !soap_valid_socket( m ) ) {
        return 1;
    };

    SOAPService::Controller< ReplicationApiServiceImpl > controller( service );

    if( !controller.start() ) {
        return 2;
    }

    return app.exec();
}

