#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

#include "asktopairdialog.h"
#include "paircodedialog.h"

#include "connectionmanager.h"
#include "paireddevicesmodel.h"
#include "founddevicesmodel.h"

namespace Ui {
class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();
    
private:
    Ui::MainWindow *ui;
    AskToPairDialog *askToPairDialog;
    PairCodeDialog *pairCodeDialog;

    ConnectionManager *connectionManager;
    PairedDevicesModel *pairedDevicesModel;
    FoundDevicesModel *foundDevicesModel;

private slots:
    /**
      * ConnectionManager - Device requests pairing -> Show AskToPair
      */
    void deviceWantsToPair(network_id_ptr id, QHostAddress ip);

    /**
      * ConnectionManager - Device paired.
      */
    void devicePaired(network_id_ptr id, QHostAddress ip, bool successful);

    /**
      * ConnectionManager - Pairing timed out -> close pairing dialogs.
      */
    void pairTimedOut();

    /**
      * AskToPairDialog return result -> stop pairing or show PairCodeDialog.
      */
    void deviceAllowedToPair(bool yes);

    /**
      * Optional cancel button bit in PairCodeDialog -> send cancel message to device.
      */
    void pairCanceledServerSide();
};

#endif // MAINWINDOW_H
