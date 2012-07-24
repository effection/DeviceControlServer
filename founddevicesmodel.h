#ifndef FOUNDDEVICESMODEL_H
#define FOUNDDEVICESMODEL_H

#include <QAbstractListModel>
#include <QList>
#include <QHostAddress>
#include "networkid.h"

class FoundDevicesModel : public QAbstractListModel
{
    Q_OBJECT

public:
    FoundDevicesModel(QObject *parent = 0);

    int rowCount(const QModelIndex &parent = QModelIndex()) const;
    QVariant data(const QModelIndex &index, int role = Qt::DisplayRole) const;

    void setDeviceNames(QHash<network_id_ptr, QHostAddress> names);

public slots:
    void deviceFound(network_id_ptr id, QHostAddress ip);
    void foundDevicesCleared();

private:
    QHash<network_id_ptr, QHostAddress> devices;
    QHash<network_id_ptr, QString> deviceNames;
};

#endif // FOUNDDEVICESMODEL_H
