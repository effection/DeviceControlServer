#-------------------------------------------------
#
# Project created by QtCreator 2012-07-13T20:50:21
# iphlpapi.lib
#-------------------------------------------------

QT       += core gui
QT       += network
TARGET = DesktopControlServer
TEMPLATE = app


SOURCES += main.cpp\
        mainwindow.cpp \
    udpconnection.cpp \
    connectionmanager.cpp \
    packetreceiveddata.cpp

HEADERS  += mainwindow.h \
    udpconnection.h \
    connectionmanager.h \
    packetreceiveddata.h \
    MacAddress.h \
    networkid.h \
    bihash.h

FORMS    += mainwindow.ui

LIBS += -lws2_32
win32:LIBS += C:\QtSDK\mingw\lib\libiphlpapi.a
