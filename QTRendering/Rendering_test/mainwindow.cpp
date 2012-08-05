#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::setVersion(std::string version)
{
    ui->versionLbl->setText(QString::fromStdString(version));
}


void MainWindow::startRendering(CDOH platformHandle, std::string sinkId)
{
    ui->renderContainer->setPlatformHandle(platformHandle);
    ui->renderContainer->startRender(sinkId);
}

