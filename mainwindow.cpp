#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
    connectionManager = new ConnectionManager(this);
    connectionManager->start();
}

MainWindow::~MainWindow()
{
    delete ui;
}
