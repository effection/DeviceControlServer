#include "connectionmanager.h"
#include <QTime>
#include <qglobal.h>

ConnectionManager::ConnectionManager(QObject *parent) :
    QObject(parent)
{
    discoveryConnection = new UdpConnection(this);
    commConnection = new UdpConnection(this);
    dataConnection = new UdpConnection(this);

    myMacAddress = MacAddress::myMacAddress();

    pairTimer = new QTimer(this);
    pairTimer->setSingleShot(true);
    pairTimer->setInterval(PAIR_TIMEOUT);
    connect(pairTimer, SIGNAL(timeout()), this, SLOT(pairingTimedOut()));

    //Seed random
    QTime time = QTime::currentTime();
    qsrand((uint)time.msec());

    connect(discoveryConnection, SIGNAL(packetReceived(PacketReceivedData)),
            this, SLOT(discoveryMessageReceived(PacketReceivedData)));

    connect(commConnection, SIGNAL(packetReceived(PacketReceivedData)),
            this, SLOT(commMessageReceived(PacketReceivedData)));

    connect(dataConnection,SIGNAL(packetReceived(PacketReceivedData)),
            this, SLOT(dataMessageReceived(PacketReceivedData)));
}

void ConnectionManager::start()
{
    discoveryConnection->startListening(DISCOVERY_ADDR, DISCOVERY_PORT);
    commConnection->startListening(COMM_ADDR, COMM_PORT);
    dataConnection->startListening(DATA_ADDR, DATA_PORT);
}

void ConnectionManager::stop()
{
    discoveryConnection->stopListening();
    commConnection->stopListening();
    dataConnection->stopListening();
}

void ConnectionManager::reportDevicesOnNetwork()
{
    QByteArray mac = myMacAddress->getBytes();
    QByteArray bytes;
    bytes.append(ARE_YOU_A_SERVER_RESPONSE);
    bytes.append(mac);
    discoveryConnection->send(DISCOVERY_ADDR, DISCOVERY_PORT, bytes);
}

QString ConnectionManager::generatePairCode(int len)
{
    QString code;
    pairingInfo.code.clear();

    for(int i = 0; i < len; i++)
    {
        int r = randInt(0,9);
        pairingInfo.code.append(r);
        code.append(r);
    }
    return code;
}

void ConnectionManager::allowDeviceToPair(network_id_ptr id, bool allowed)
{
    if(id != pairingInfo.device || pairTimer->isActive())
        return;
    if (allowed)
    {
        //Do nothing. Handler has already done a GeneratePairCode()
        //Waiting on PAIR_CODE_RESPONSE now
        pairTimer->start();
    }
    else
    {
        commConnection->send(pairingInfo.ip, pairingInfo.port, PAIR_CODE_CANCEL);
        stopPairing();
    }
}

void ConnectionManager::addFoundDevice(network_id_ptr id, QHostAddress ip)
{
    foundDevices.push_back(id);
    emit deviceFound(id);
}

void ConnectionManager::addPairedDevice(network_id_ptr id, QHostAddress ip)
{
    pairedDevices.add(id, ip);
}

void ConnectionManager::updatePairedDeviceIp(network_id_ptr id, QHostAddress ip)
{
    //Move old ip's disalloweds to new ip
    QHostAddress oldIp = pairedDevices[id];

    if(oldIp != ip)
    {
        pairedDevices[id] = ip;
#ifdef ALLOW_DISABLING_PLUGINS_FOR_SPECIFIC_DEVICES
        disallowedHandlers[ip] = disallowedHandlers[oldIp];
        disallowedHandlers.remove(oldIp);
#endif
        emit deviceIpChanged(id, ip);
    }
}

void ConnectionManager::removedPairedDevice(network_id_ptr id)
{
    pairedDevices.remove(id);
    emit deviceRemoved(id);
}

void ConnectionManager::discoveryMessageReceived(PacketReceivedData packet)
{
    QByteArray data = packet.getBytes();
    if (isMessage(ARE_YOU_A_SERVER, data))
    {
        //Send our mac address back
        QByteArray mac = myMacAddress->getBytes();
        QByteArray bytes;
        bytes.append(ARE_YOU_A_SERVER_RESPONSE);
        bytes.append(mac);
        discoveryConnection->send(DISCOVERY_ADDR, DISCOVERY_PORT, bytes);
    }
}

void ConnectionManager::commMessageReceived(PacketReceivedData packet)
{
    QByteArray data = packet.getBytes();
    QHostAddress ip = packet.getAddress();
    int port = packet.getPort();
    network_id_ptr mac = MacAddress::getMacAddress(ip);


    if (isMessage(ASK_TO_PAIR, data) && !pairingInfo.isPairing)
    {
        if (pairedDevices.containsKey(mac))
        {
            //Requesting device already paired
            commConnection->send(ip, port, ALREADY_PAIRED);
            return;
        }else
        {
            //Requesting device not paired yet
            commConnection->send(ip, port, OK_TO_PAIR);
            startPairing(ip, port, mac);
        }
    }
    else if (isMessage(PAIR_CODE_RESPONSE, data) && pairingInfo.isPairing)
    {
        //Stop other devices trying to send us the pair code or empty pair codes
        if(pairingInfo.device != mac || pairingInfo.code.size() <= 0)
            return;

        //Sending pair code to us
        //var sentCode = new byte[pairCode.Length];
        //Array.Copy(e.Bytes, PAIR_CODE_RESPONSE.Length, sentCode, 0, pairCode.Length);
        int startIdx = MESSAGE_SIZE;
        bool sentCodeCorrect = true;
        for(int i = startIdx, j = 0;
            i < data.length() && j < pairingInfo.code.length();
            i++, j++)
        {
            if(data[i] != pairingInfo.code[j])
            {
                sentCodeCorrect = false;
                break;
            }
        }

        stopPairing();

        if (sentCodeCorrect)
        {
            addPairedDevice(mac, ip);
            commConnection->send(ip, port, AUTHORISED);
            emit devicePaired(mac, ip, true);
        }
        else
        {
            commConnection->send(ip, port, INCORRECT_PAIR_CODE);
            emit devicePaired(mac, ip, false);
        }
    }
    else if (isMessage(PAIR_CODE_CANCEL, data) && pairingInfo.isPairing)
    {
        stopPairing();
        emit pairCanceled();
    }
    else if (isMessage(ARE_WE_PAIRED, data) && !pairingInfo.isPairing)
    {
        //Check existing ip/hardware id pairs to see if we can ignore it
        if (pairedDevices.containsKey(mac))
        {
            //If the hardware ID already exists the ip should be updated!
            updatePairedDeviceIp(mac, ip);
        }
        else
        {
            if(!pairedDevices.containsValue(ip))
                //No previous knowledge of this ip address or mac
                addFoundDevice(mac, ip);
        }

        //Report back to the device
        if (pairedDevices.containsKey(mac))
            commConnection->send(ip, port, ALREADY_PAIRED);
        else
            commConnection->send(ip, port, NOT_PAIRED);
    }
}

void ConnectionManager::dataMessageReceived(PacketReceivedData packet)
{
    QByteArray data = packet.getBytes();
    QHostAddress ip = packet.getAddress();

    //Check if allowed hardware
    if (!pairedDevices.containsValue(ip))
    {
        emit unauthorisedDevice(pairedDevices[ip], ip);
        return;
    }

    for(uint i = 0; i < packetHandlers.size(); i++)
    {
#ifdef ALLOW_DISABLING_PLUGINS_FOR_SPECIFIC_DEVICES
        if(disallowedHandlers.contains(ip)
           && !disallowedHandlers[ip].contains(packetHandlers[i]->id()))
            continue;
#endif

        if (packetHandlers[i]->handle(data))
            break;
    }
}

void ConnectionManager::startPairing(QHostAddress ip, int port, network_id_ptr mac)
{
    pairingInfo.isPairing = true;
    pairingInfo.ip = ip;
    pairingInfo.port = port;
    //Increases ref count
    pairingInfo.device = mac;
    QString pairCode = generatePairCode(6);
    emit deviceWantsToPair(mac, ip);
}

void ConnectionManager::stopPairing()
{
    pairingInfo.isPairing = false;
    pairingInfo.code.clear();
    pairingInfo.ip.clear();
    pairingInfo.port = 0;
    pairingInfo.device.reset();
}

void ConnectionManager::pairingTimedOut()
{
    commConnection->send(pairingInfo.ip, pairingInfo.port, PAIR_CODE_CANCEL);
    stopPairing();
    emit pairTimedOut();
}

bool ConnectionManager::isMessage(const char msg[], QByteArray bytes)
{
    for(int i = 0; i < 1; i++)
    {
        if(msg[i] != bytes.data()[i])
            return false;
    }
    return true;
}

int ConnectionManager::randInt(int low, int high)
{
    // Random number between low and high
    return qrand() % ((high + 1) - low) + low;
}

const QHostAddress ConnectionManager::DISCOVERY_ADDR = QHostAddress("224.0.0.86");
const QHostAddress ConnectionManager::COMM_ADDR = QHostAddress::Any;
const QHostAddress ConnectionManager::DATA_ADDR = QHostAddress::Any;

const char ConnectionManager::IS_ANYONE_THERE[] = {'a'};
const char ConnectionManager::ALREADY_PAIRED[] = { 'b' };
const char ConnectionManager::NOT_PAIRED[] = { 'c' };
const char ConnectionManager::AUTHORISED[] = { 'd' };
const char ConnectionManager::INCORRECT_PAIR_CODE[] = { 'e' };
const char ConnectionManager::ARE_YOU_A_SERVER_RESPONSE[] = { 'f' };
const char ConnectionManager::OK_TO_PAIR[] = { 'm' };

const char ConnectionManager::IS_ANYONE_THERE_RESPONSE[] = { 'g' };
const char ConnectionManager::ASK_TO_PAIR[] = { 'h' };
const char ConnectionManager::PAIR_CODE_RESPONSE[] = { 'i' };
const char ConnectionManager::PAIR_CODE_CANCEL[] = { 'j' };
const char ConnectionManager::ARE_YOU_A_SERVER[] = { 'k' };
const char ConnectionManager::ARE_WE_PAIRED[] = { 'l' };
