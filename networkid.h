#ifndef NETWORKID_H
#define NETWORKID_H

#include <QByteArray>
#include <QHostAddress>

class NetworkId
{
public:
    NetworkId() {}
    virtual ~NetworkId() {}

    virtual QByteArray getBytes() const
    {
        return bytes;
    }
protected:
    QByteArray bytes;
};

inline bool operator==(const NetworkId &nid1, const NetworkId &nid2)
{
    return nid1.getBytes() == nid2.getBytes();
}

inline uint qHash(const NetworkId &id)
{
    QByteArray arr = id.getBytes();
    return qHash(arr);
}
#endif // NETWORKID_H
