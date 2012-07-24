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
    packetreceiveddata.cpp \
    paireddevicesmodel.cpp \
    founddevicesmodel.cpp \
    asktopairdialog.cpp \
    paircodedialog.cpp \
    macaddress.cpp

HEADERS  += mainwindow.h \
    udpconnection.h \
    connectionmanager.h \
    packetreceiveddata.h \
    networkid.h \
    bihash.h \
    macaddress.h \
    ipackethandler.h \
    paireddevicesmodel.h \
    founddevicesmodel.h \
    asktopairdialog.h \
    paircodedialog.h

FORMS    += mainwindow.ui \
    asktopairdialog.ui \
    paircodedialog.ui

LIBS += -lws2_32
win32:LIBS += C:\QtSDK\mingw\lib\libiphlpapi.a
