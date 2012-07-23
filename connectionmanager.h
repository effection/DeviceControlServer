#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QObject>
#include <QTimer>
#include <QList>
#include <memory>
#include "udpconnection.h"
#include "packetreceiveddata.h"
#include "MacAddress.h"
#include "bihash.h"

using namespace std::tr1;

typedef shared_ptr<NetworkId> network_id_ptr;

typedef struct _PairingInfo
{
    bool isPairing;
    QHostAddress ip;
    int port;
    network_id_ptr device;
    QByteArray code;
} PairingInfo;

inline uint qHash(const QHostAddress &addr)
{
    return qHash(addr.toString());
}

inline uint qHash(const network_id_ptr &idPtr)
{
    return qHash(idPtr.get());
}

class ConnectionManager : public QObject
{
    Q_OBJECT
public:
    explicit ConnectionManager(QObject *parent = 0);
    
    /**
      * Start all connections.
      */
    void start();

    /**
      * Stop all connections.
      */
    void stop();

    /**
      * Ask the devices on the network to report in.
      */
    void reportDevicesOnNetwork();

    /**
      * Create a pair code ready to be sent from the device to authorise it.
      */
    QString generatePairCode(int len);

signals:
    
public slots:

private:
    /**
      * Store pairing information.
      */
    void startPairing(QHostAddress ip, int port, network_id_ptr hardwareId);

    /**
      * Stop pairing.
      */
    void stopPairing();

    void addFoundDevice(network_id_ptr id, QHostAddress ip);
    void addPairedDevice(network_id_ptr id, QHostAddress ip);
    void updatePairedDeviceIp(network_id_ptr id, QHostAddress ip);
    void removedPairedDevice(network_id_ptr id);

    static bool isMessage(const char msg[], QByteArray bytes);
    static int randInt(int low, int high);

private slots:
    void discoveryMessageReceived(PacketReceivedData packet);
    void commMessageReceived(PacketReceivedData packet);
    void dataMessageReceived(PacketReceivedData packet);

    void pairingTimedOut();

private:
    UdpConnection *discoveryConnection;
    UdpConnection *commConnection;
    UdpConnection *dataConnection;

    QTimer *pairTimer;

    network_id_ptr myMacAddress;

    BiHash<network_id_ptr, QHostAddress> pairedDevices;

    PairingInfo pairingInfo;

    static const QHostAddress DISCOVERY_ADDR;
    static const qint16 DISCOVERY_PORT = 11300;
    static const QHostAddress COMM_ADDR;
    static const qint16 COMM_PORT = 11301;
    static const QHostAddress DATA_ADDR;
    static const qint16 DATA_PORT = 11302;

    //Message sequences
    //TO DEVICE
            static const char IS_ANYONE_THERE[];//{ 0xF0, 0xFF, 0x00 };
            static const char ALREADY_PAIRED[];//{ 0xF0, 0xFF, 0x01 };
            static const char NOT_PAIRED[];//{ 0xF0, 0xFF, 0x02 };
            static const char AUTHORISED[];//{ 0xF0, 0xFF, 0x03 };
            static const char INCORRECT_PAIR_CODE[];//{ 0xF0, 0xFF, 0x04 };
            //private static readonly byte[] PAIR_CODE_CANCEL =           { 'i' };//{ 0xF0, 0xFF, 0x08 };
            static const char ARE_YOU_A_SERVER_RESPONSE[];
            static const char OK_TO_PAIR[];

            //FROM DEVICE
            static const char IS_ANYONE_THERE_RESPONSE[];//{ 0xF0, 0xFF, 0x05 };
            static const char ASK_TO_PAIR[];//{ 0xF0, 0xFF, 0x06 };
            static const char PAIR_CODE_RESPONSE[];//{ 0xF0, 0xFF, 0x07 };
            static const char PAIR_CODE_CANCEL[];//{ 0xF0, 0xFF, 0x08 };
            static const char ARE_YOU_A_SERVER[];
            static const char ARE_WE_PAIRED[];
    
};

#endif // CONNECTIONMANAGER_H
