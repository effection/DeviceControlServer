#ifndef network_id_ptr_H
#define network_id_ptr_H

#include <QByteArray>
#include <memory>

using namespace std::tr1;

class NetworkId
{
public:
    NetworkId() {}
    virtual ~NetworkId() {}

    bool operator==(const NetworkId& rhs) const
    {
        QByteArray rhsb = rhs.getBytes();
        if(rhsb.size() != this->bytes.size())
            return false;
        for(int i = 0; i < rhsb.size(); i++)
            if(rhsb.data()[i] != this->bytes.data()[i])
                return false;
        return true;
    }

    bool operator!=(const NetworkId& rhs) const
    {
        return !operator==(rhs);
    }

    virtual QByteArray getBytes() const
    {
        return bytes;
    }

    virtual QByteArray getBytes()
    {
        return bytes;
    }

    virtual QString toString() const
    {
        return QString();
    }

    virtual QString toString()
    {
        return QString();
    }

protected:
    QByteArray bytes;
};

typedef shared_ptr<NetworkId> network_id_ptr;


inline bool operator==(const network_id_ptr &nid1, const network_id_ptr &nid2)
{
    return nid1.get() == nid2.get();
}

inline uint qHash(const NetworkId &id)
{
    QByteArray arr = id.getBytes();
    return qHash(arr);
}

inline uint qHash(const network_id_ptr &idPtr)
{
    return qHash(idPtr.get());
}

#endif // network_id_ptr_H
