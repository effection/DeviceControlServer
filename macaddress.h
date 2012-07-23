#ifndef MACADDRESS_H
#define MACADDRESS_H

#include <QByteArray>
#include <QHostAddress>
#include <QHostInfo>
#include <memory>
#include "networkid.h"

#ifdef Q_WS_WIN
#include <winsock2.h>
#include <iphlpapi.h>
#endif

using namespace std::tr1;

class MacAddress : public NetworkId
{
public:
    MacAddress(QByteArray bytes) { this->bytes = bytes; }

    QString getString() const
    {
        return QString();

    }

    static QHostAddress myIpv4Address()
    {
        QHostInfo info;
        QString qstr = info.localHostName();
        qDebug() << qstr << endl;
        QHostInfo info2 ( QHostInfo ::fromName ( qstr ) );
        QList <QHostAddress> hostaddr = info2.addresses();
        for ( QList<QHostAddress> ::const_iterator it = hostaddr.begin(); it != hostaddr.end(); ++it )
        {
            QHostAddress x = *it;
            if(x.protocol() == QAbstractSocket::IPv4Protocol)
            {
                return x;
            }
        }
        return QHostAddress::Null;
    }

    static shared_ptr<MacAddress> myMacAddress()
    {
        QByteArray bytes;
        QHostAddress myIp = myIpv4Address();
        if(myIp != QHostAddress::Null)
        {
            return getMacAddress(myIp);
        }
        return shared_ptr<MacAddress>(new MacAddress(bytes));
    }

    static shared_ptr<MacAddress> getMacAddress(QHostAddress ip)
    {
        QByteArray bytes;

#ifdef Q_WS_WIN
        IPAddr destIp;
        qDebug() << ip.toString().toStdString().c_str() << endl;
        destIp = inet_addr(ip.toString().toStdString().c_str());
        ULONG macAddr[2];
        ULONG physAddrLen = 6;
        memset(&macAddr, 0xff, sizeof (macAddr));

        int ret = SendARP(destIp, 0, &macAddr, &physAddrLen);
        qDebug() << ret << endl;

        for (ULONG i = 0; i < physAddrLen; i++) {
            bytes.append(*((char*)&macAddr + i));
        }

#endif
        return shared_ptr<MacAddress>(new MacAddress(bytes));
    }
};

#endif // MACADDRESS_H
