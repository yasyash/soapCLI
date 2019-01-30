QT       += core sql

QT       -= gui

TARGET = ReplicationApiService
VERSION = 0.1.0

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    soapC.cpp \
    soapReplicationApiServiceService.cpp \
    replicationapiserviceimpl.cpp \
    soapservice.cpp

HEADERS += \
    soapStub.h \
    soapH.h \
    soapReplicationApiServiceService.h \
    ReplicationApiService.nsmap \
    replicationapiserviceimpl.h \
    soapservice.h \
    stdsoap2.h \
    global.h

DEFINES += WITH_PURE_VIRTUAL\
           WITH_NOIDREF

include( common.pri )
