QT       += core sql

QT       -= gui

TARGET = cli_soap
VERSION = 0.1.0

CONFIG   += console
CONFIG   -= app_bundle

TEMPLATE = app

SOURCES += main.cpp \
    soapC.cpp \
    soapReplicationApiServiceProxy.cpp \
    wiper.cpp

HEADERS += \
    soapStub.h \
    soapH.h \
    ReplicationApiService.nsmap \
    stdsoap2.h \
    soapReplicationApiServiceProxy.h \
    wiper.h \
    global.h

DEFINES += WITH_PURE_VIRTUAL\
           WITH_NOIDREF

include( common.pri )


