#ifndef PACKETRECEIVEDDATA_H
#define PACKETRECEIVEDDATA_H

#include <QHostAddress>

class PacketReceivedData
{
public:
    PacketReceivedData(QHostAddress addr, int port, QByteArray bytes);

    QHostAddress getAddress() const;
    int getPort() const;
    QByteArray getBytes() const;

private:
    QHostAddress address;
    int port;
    QByteArray bytes;
};

#endif // PACKETRECEIVEDDATA_H
