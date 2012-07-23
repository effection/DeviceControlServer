#include "udpconnection.h"

UdpConnection::UdpConnection(QObject *parent)
    : QObject(parent)
{
    socket = new QUdpSocket(this);
    connect(socket, SIGNAL(readyRead()), this, SLOT(processPendingDatagrams()));
}

UdpConnection::~UdpConnection()
{
    if(_isMulticast)
        socket->leaveMulticastGroup(multicastAddress, multicastInterface);
}

bool UdpConnection::joinMulticastGroup(QHostAddress address, QNetworkInterface interface)
{
    if(_isMulticast)
        return (multicastAddress == address);

    _isMulticast = socket->joinMulticastGroup(address, interface);

    if(_isMulticast)
    {
        multicastAddress = address;
        multicastInterface = interface;
    }
    return _isMulticast;
}

bool UdpConnection::send(QHostAddress address, qint16 port, QByteArray data)
{
    return socket->writeDatagram(data.data(), data.size(), address, port) != -1;
}

bool UdpConnection::startListening(QHostAddress address, qint16 port)
{
    isListening = socket->bind(address, port);
    return isListening;
}

void UdpConnection::stopListening()
{
    isListening = false;
}

bool UdpConnection::isMulticast() const
{
    return _isMulticast;
}

void UdpConnection::processPendingDatagrams()
{
    if(!isListening)
        return;

    while (socket->hasPendingDatagrams())
    {
        QByteArray datagram;
        datagram.resize(socket->pendingDatagramSize());
        QHostAddress sender;
        quint16 senderPort;
        socket->readDatagram(datagram.data(), datagram.size(), &sender, &senderPort);
        //qDebug() << "Address: " << sender << " - " << senderPort << endl;
        emit packetReceived(PacketReceivedData(sender, senderPort, datagram) );
    }
}
