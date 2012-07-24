#ifndef PAIREDDEVICESMODEL_H
#define PAIREDDEVICESMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QHash>
#include <QHostAddress>
#include "networkid.h"

class PairedDevicesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    PairedDevicesModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void setDeviceNames(QHash<network_id_ptr, QString> names);

public slots:
    void devicePaired(network_id_ptr id, QHostAddress ip, bool successful);
    void deviceRemoved(network_id_ptr id);

protected:


private:
    QList<network_id_ptr> pairedDevices;
    QHash<network_id_ptr, QString> deviceNames;
};

#endif // PAIREDDEVICESMODEL_H
