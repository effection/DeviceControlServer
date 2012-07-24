#ifndef CONNECTIONMANAGER_H
#define CONNECTIONMANAGER_H

#include <QObject>
#include <QTimer>
#include <QList>
#include <QUuid>
#include <memory>
#include "bihash.h"
#include "packetreceiveddata.h"
#include "udpconnection.h"
#include "macaddress.h"
#include "ipackethandler.h"

//#define ALLOW_DISABLING_PLUGINS_FOR_SPECIFIC_DEVICES

using namespace std::tr1;

typedef shared_ptr<IPacketHandler> packet_handler_ptr;

typedef struct _PairingInfo
{
    bool isPairing;
    QHostAddress ip;
    int port;
    network_id_ptr device;
    QByteArray code;

public:
    _PairingInfo() :
        isPairing(false), port(0)
    {
    }

} PairingInfo;

inline uint qHash(const QHostAddress &addr)
{
    return qHash(addr.toString());
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

    /**
      * Allows the requesting device to pair.
      */
    void allowDeviceToPair(bool yes);

    /**
      * Remove paired device from list.
      */
    void removedPairedDevice(network_id_ptr id);

    /**
      * Add a handler for specific packets (first come first serve basis).
      */
    void addPacketHandler(packet_handler_ptr handler);

signals:

    void deviceWantsToPair(network_id_ptr id, QHostAddress ip);
    void devicePaired(network_id_ptr id, QHostAddress ip, bool successful);
    void pairCanceled();
    void pairTimedOut();
    void unauthorisedDevice(network_id_ptr id, QHostAddress ip);

    void foundDevicesCleared();
    void deviceFound(network_id_ptr id, QHostAddress ip);
    void deviceIpChanged(network_id_ptr id, QHostAddress newIp);
    void deviceRemoved(network_id_ptr id);
    
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

    /**
      * Store information about found device on network.
      */
    void addFoundDevice(network_id_ptr id, QHostAddress ip);

    /**
      * Store information about a paired device.
      */
    void addPairedDevice(network_id_ptr id, QHostAddress ip);

    /**
      * Update paired device's associated MAC address.
      */
    void updatePairedDeviceIp(network_id_ptr id, QHostAddress ip);

    /**
      * Helper to determine packet type.
      */
    static bool isMessage(const char msg[], QByteArray bytes);

    /**
      * Helper to generate random integer.
      */
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

    std::vector<packet_handler_ptr> packetHandlers;
#ifdef ALLOW_DISABLING_PLUGINS_FOR_SPECIFIC_DEVICES
    QHash<QHostAddress, QList<QUuid> > disallowedHandlers;
#endif

    std::vector<network_id_ptr> foundDevices;
    BiHash<network_id_ptr, QHostAddress> pairedDevices;
    PairingInfo pairingInfo;

    static const int PAIR_TIMEOUT = 30 * 1000;

    static const QHostAddress DISCOVERY_ADDR;
    static const qint16 DISCOVERY_PORT = 11300;
    static const QHostAddress COMM_ADDR;
    static const qint16 COMM_PORT = 11301;
    static const QHostAddress DATA_ADDR;
    static const qint16 DATA_PORT = 11302;

    //Message sequences
    //TO DEVICE
    static const int MESSAGE_SIZE = 1;
    static const QByteArray IS_ANYONE_THERE_B[];
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
