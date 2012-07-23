#ifndef ACTIVEDEVICEMAP_H
#define ACTIVEDEVICEMAP_H

#include <QHash>

template<class TKey, class TValue>
class BiHash
{
public:
    BiHash()
    {

    }

    void add(const TKey key, const TValue value)
    {
        keyToValueMap[key] = value;
        valueToKeyMap[value] = key;
    }

    void remove(const TKey key)
    {
        TValue value = keyToValueMap[key];
        keyToValueMap.remove(key);
        valueToKeyMap.remove(value);
    }

    inline bool containsKey(const TKey id)
    {
        return keyToValueMap.contains(id);
    }

    inline bool containsValue(const TValue value)
    {
        return valueToKeyMap.contains(value);
    }

    inline bool empty()
    {
        return keyToValueMap.empty();
    }

    inline int size()
    {
        return keyToValueMap.size();
    }

    TValue& operator[](TKey key){ return keyToValueMap[key];}
    const TValue& operator[](TKey key) const { return keyToValueMap[key];}

    TKey& operator[](TValue value){ return valueToKeyMap[value];}
    const TKey& operator[](TValue value) const { return valueToKeyMap[value];}

private:
    QHash<TKey, TValue> keyToValueMap;
    QHash<TValue, TKey> valueToKeyMap;
};

#endif // ACTIVEDEVICEMAP_H
