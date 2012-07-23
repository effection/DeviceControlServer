#ifndef IPACKETHANDLER_H
#define IPACKETHANDLER_H

#include <QByteArray>
#include <QUuid>

class IPacketHandler
{
public:
    IPacketHandler() {}
    virtual ~IPacketHandler() {}

    virtual QUuid id() = 0;

    virtual bool shouldHandle(QByteArray packet) = 0;

    virtual bool handle(QByteArray packet) = 0;
};

#endif // IPACKETHANDLER_H
