#ifndef UDPCONNECTION_H
#define UDPCONNECTION_H

#include <QHostAddress>
#include <QNetworkInterface>
#include <QString>
#include <QUdpSocket>
#include "packetreceiveddata.h"


class UdpConnection : public QObject
{
    Q_OBJECT

public:
    UdpConnection(QObject *parent = 0);
    ~UdpConnection();

    bool joinMulticastGroup(QHostAddress address, quint16 port);

    bool send(QHostAddress address, qint16 port, QByteArray data);
    bool startListening(QHostAddress address, qint16 port);
    void stopListening();

    bool isMulticast() const;

signals:
    void packetReceived(PacketReceivedData packet);

private:
    QUdpSocket *socket;
    QHostAddress multicastAddress;
    QNetworkInterface multicastInterface;

    bool isListening;
    bool _isMulticast;

private slots:
    void processPendingDatagrams();

};

#endif // UDPCONNECTION_H
