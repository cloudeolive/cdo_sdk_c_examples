#include "cdosampleappwindow.h"
#include "ui_cdosampleappwindow.h"
#include <QDebug>

#include <boost/foreach.hpp>
CdoSampleAppWindow::CdoSampleAppWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::CdoSampleAppWindow)
{
    ui->setupUi(this);
    setupBindings();
    _appController.initCDO();
}


CdoSampleAppWindow::~CdoSampleAppWindow()
{
    delete ui;
}

void CdoSampleAppWindow::onMediaDevicesListChanged(int devType,
                                                   QVariantMap devs)
{
    QComboBox * targetCombo;
    switch(devType)
    {
    case AUDIO_IN:
        targetCombo = ui->micCombo;
        break;
    case AUDIO_OUT:
        targetCombo = ui->spkCombo;
        break;
    case VIDEO_IN:
        targetCombo = ui->camCombo;
        break;
    }

    targetCombo->clear();
    QVariantMap::iterator i;
    for(i=devs.begin(); i!= devs.end();i++)
    {

        qDebug() << "Adding device: " << i.value();
        targetCombo->addItem(QIcon(), i.value().toString() ,i.key() );
    }
}

void CdoSampleAppWindow::onCDOPlatformReady(void* pH, QString v)
{
    ui->versionLbl->setText(v);
    ui->localRenderer->setPlatformHandle(pH);
    ui->remoteRenderer->setPlatformHandle(pH);
}

void CdoSampleAppWindow::onLocalPreviewSinkChanged(QString sinkId)
{
    ui->localRenderer->stopRender();
    ui->localRenderer->startRender(sinkId.toStdString());
}


void CdoSampleAppWindow::setupBindings()
{
    QObject::connect(ui->playTestSndBtn, SIGNAL(clicked()),
                     &_appController,  SLOT(playTestSndClicked()));


    QObject::connect(&_appController, SIGNAL(cdoReady(void*,QString)),
                     this,SLOT(onCDOPlatformReady(void*,QString)));

    QObject::connect(&_appController,
                     SIGNAL(mediaDevicesListChanged(int,QVariantMap)),
                     this, SLOT(onMediaDevicesListChanged(int,QVariantMap)));
    QObject::connect(&_appController,
                     SIGNAL(localVideoSinkChanged(QString)),
                     this, SLOT(onLocalPreviewSinkChanged(QString)));
}




