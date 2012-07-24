#include "paireddevicesmodel.h"

PairedDevicesModel::PairedDevicesModel(QObject *parent)
    : QAbstractListModel(parent)
{
}

int PairedDevicesModel::rowCount(const QModelIndex & /* parent */) const
{
    return pairedDevices.size();
}

QVariant PairedDevicesModel::data(const QModelIndex &index, int role) const
{
    if (!index.isValid())
        return QVariant();

    if (index.row() >= pairedDevices.size() || index.row() < 0)
        return QVariant();

    if (role == Qt::DisplayRole)
    {
        network_id_ptr item = pairedDevices.at(index.row());
        QString displayName;
        if(deviceNames.contains(item))
        {
            displayName = deviceNames[item];
        }else
        {
            displayName = item->toString();
        }
        return displayName;
    }

    return QVariant();
}

void PairedDevicesModel::setDeviceNames(QHash<network_id_ptr, QString> names)
{
    deviceNames = names;
}

void PairedDevicesModel::devicePaired(network_id_ptr id, QHostAddress ip, bool successful)
{
    if(successful)
    {
        if(pairedDevices.contains(id))
            return;

        beginInsertRows(QModelIndex(), pairedDevices.size(), pairedDevices.size());
        pairedDevices.push_back(id);
        endInsertRows();

    }else
    {
        int index = pairedDevices.indexOf(id);
        beginRemoveRows(QModelIndex(), index, index);
        pairedDevices.removeOne(id);
        endRemoveRows();
    }
}

void PairedDevicesModel::deviceRemoved(network_id_ptr id)
{
    int index = pairedDevices.indexOf(id);
    beginRemoveRows(QModelIndex(), index, index);
    pairedDevices.removeOne(id);
    endRemoveRows();
}
