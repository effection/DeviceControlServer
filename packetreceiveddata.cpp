#include "packetreceiveddata.h"

PacketReceivedData::PacketReceivedData(QHostAddress addr, int port, QByteArray bytes)
{
    this->address = addr;
    this->port = port;
    this->bytes = bytes;
}

QHostAddress PacketReceivedData::getAddress() const
{
    return address;
}

int PacketReceivedData::getPort() const
{
    return port;
}

QByteArray PacketReceivedData::getBytes() const
{
    return bytes;
}
