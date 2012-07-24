#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    connectionManager = new ConnectionManager(this);
    connect(connectionManager, SIGNAL(deviceWantsToPair(network_id_ptr,QHostAddress)),
            this, SLOT(deviceWantsToPair(network_id_ptr,QHostAddress)));
    connect(connectionManager, SIGNAL(devicePaired(network_id_ptr,QHostAddress,bool)),
            this, SLOT(devicePaired(network_id_ptr,QHostAddress,bool)));
    connect(connectionManager, SIGNAL(pairTimedOut()),
            this, SLOT(pairTimedOut()));


    pairedDevicesModel = new PairedDevicesModel(this);
    connect(connectionManager, SIGNAL(devicePaired(network_id_ptr,QHostAddress,bool)),
            pairedDevicesModel, SLOT(devicePaired(network_id_ptr,QHostAddress,bool)));

    connect(connectionManager, SIGNAL(deviceRemoved(network_id_ptr)),
            pairedDevicesModel, SLOT(deviceRemoved(network_id_ptr)));

    foundDevicesModel = new FoundDevicesModel(this);
    connect(connectionManager, SIGNAL(deviceFound(network_id_ptr,QHostAddress)),
            foundDevicesModel, SLOT(deviceFound(network_id_ptr,QHostAddress)));

    connect(connectionManager, SIGNAL(foundDevicesCleared()),
            foundDevicesModel, SLOT(foundDevicesCleared()));

    //pairedDevicesModel->setDeviceNames();
    //foundDevicesModel->setDeviceNames();

    askToPairDialog = new AskToPairDialog(this);
    connect(askToPairDialog, SIGNAL(allowedToPair(bool)),
            this, SLOT(deviceAllowedToPair(bool)));

    pairCodeDialog = new PairCodeDialog(this);
    connect(pairCodeDialog, SIGNAL(canceled()),
            this, SLOT(pairCanceledServerSide()));

    connect(connectionManager, SIGNAL(devicePaired(network_id_ptr,QHostAddress,bool)),
            pairCodeDialog, SLOT(devicePaired(network_id_ptr,QHostAddress,bool)));
    connect(connectionManager, SIGNAL(pairCanceledServerSide()),
            pairCodeDialog, SLOT(pairCanceledByDevice()));

    ui->lst_PairedDevices->setModel(pairedDevicesModel);
    ui->lst_FoundDevices->setModel(foundDevicesModel);

    connectionManager->start();
}

MainWindow::~MainWindow()
{
    connectionManager->stop();
    delete connectionManager;
    delete pairedDevicesModel;
    delete foundDevicesModel;

    delete ui;
    delete askToPairDialog;
}

void MainWindow::deviceAllowedToPair(bool yes)
{
    if(yes)
    {
        QString pairCode = connectionManager->generatePairCode(6);
        pairCodeDialog->setPairCode(pairCode);
        pairCodeDialog->showNormal();
    }

    connectionManager->allowDeviceToPair(yes);
}

void MainWindow::pairCanceledServerSide()
{
    //Cancel button pressed on server side
    //Cancel pairing with device
    connectionManager->allowDeviceToPair(false);
}

void MainWindow::deviceWantsToPair(network_id_ptr id, QHostAddress ip)
{
    askToPairDialog->showNormal();
}

void MainWindow::devicePaired(network_id_ptr id, QHostAddress ip, bool successful)
{

}

void MainWindow::pairTimedOut()
{
    pairCodeDialog->close();
}
