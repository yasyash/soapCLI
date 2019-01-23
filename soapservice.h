#ifndef SOAPSERVICE_H
#define SOAPSERVICE_H

#include <QSharedPointer>
#include <QThreadPool>
#include <QDebug>

#include "soapStub.h"


namespace SOAPService {

// Basic definitions

template< typename Service >
class ServeTask : public QRunnable {
public:
    ServeTask( QSharedPointer< Service > service );

    void run();

private:
    QSharedPointer< Service > m_service;

};

template< typename Service >
ServeTask< Service >::ServeTask( QSharedPointer< Service > service ) : m_service( service ) {
}

template< typename Service >
void ServeTask< Service >::run() {
    if( !m_service ) {
        return;
    }

    m_service->serve();
}

// AcceptTask

template< typename Service >
class AcceptTask : public QRunnable {
public:
    AcceptTask( QSharedPointer< Service > service, QThreadPool* threadPool );

    void run();

private:
    QSharedPointer< Service > m_service;
    QThreadPool* m_threadPool;

};

template< typename Service >
AcceptTask< Service >::AcceptTask( QSharedPointer< Service > service, QThreadPool* threadPool ) :
    m_service( service ), m_threadPool( threadPool ) {
}

template< typename Service >
void AcceptTask< Service >::run() {
    if( !m_service || !m_threadPool ) {
        return;
    }

    forever {
        SOAP_SOCKET s = m_service->accept();
        if( !soap_valid_socket( s ) ) {
            continue;
        }

        QString ip = QString( "%1.%2.%3.%4" ).
                arg( ( m_service->ip >> 24 ) & 0xFF ).
                arg( ( m_service->ip >> 16 ) & 0xFF ).
                arg( ( m_service->ip >> 8  ) & 0xFF ).
                arg( ( m_service->ip       ) & 0xFF );

        QSharedPointer< Service > serviceCopy( m_service->copy() );
        m_threadPool->start( new ServeTask< Service >( serviceCopy ) );
    }
}

template< typename Service >
class Controller {
public:
    Controller( QSharedPointer< Service > service );
    ~Controller();

    bool start();

private:
    QSharedPointer< Service > m_service;
    QThreadPool m_threadPool;

};

template< typename Service >
Controller< Service >::Controller( QSharedPointer< Service > service ) :
    m_service( service ) {
}

template< typename Service >
Controller< Service >::~Controller() {
}

template< typename Service >
bool Controller< Service >::start() {
    if( !m_service ) {
        return false;
    }

    m_threadPool.start( new AcceptTask< Service >( m_service, &m_threadPool ) );
    return true;
}


}

#endif // SOAPSERVICE_H
