#include "founddevicesmodel.h"

FoundDevicesModel::FoundDevicesModel(QObject *parent)
{
}

int FoundDevicesModel::rowCount(const QModelIndex & /* parent */) const
{
    return devices.size();
}

QVariant FoundDevicesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= devices.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        network_id_ptr id = devices.keys().at(index.row());
        QHostAddress ip = devices[id];

        QString displayName;
        if(deviceNames.contains(id))
        {
            displayName = deviceNames[id];
        }else
        {
            displayName = id->toString();
        }
        return displayName + " (" + ip.toString() + ")";
    }

    return QVariant();
}

void FoundDevicesModel::setDeviceNames(QHash<network_id_ptr, QHostAddress> names)
{
    devices = names;
}

void FoundDevicesModel::deviceFound(network_id_ptr id, QHostAddress ip)
{
    beginInsertRows(QModelIndex(), devices.size(), devices.size());
    devices[id] = ip;
    endInsertRows();
}

void FoundDevicesModel::foundDevicesCleared()
{
    devices.clear();
    this->reset();
}
