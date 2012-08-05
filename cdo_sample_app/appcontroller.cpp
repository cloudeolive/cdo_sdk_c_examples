#include "appcontroller.h"

#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <QDebug>


namespace
{
QVariantMap devsMap2QVariantMap(const std::map<std::string,std::string> in);
void nop(){}
}

AppController::AppController(QObject *parent) :
    QObject(parent)
{
}

void AppController::initCDO()
{
    qDebug() << "Initializing CDO";
    CDOReadyHandler rH = boost::bind(&AppController::onCdoReady, this, _1, _2);
    _cdoCtrl.initPlatform(rH);
}


/**
  * Slots
  ******************************************************************************
  */


void AppController::playTestSndClicked()
{
    qDebug() << "Playing test sound";
    _cdoCtrl.playTestSound();
}


/**
  * Callbacks
  ******************************************************************************
  */

void AppController::onCdoReady(CDOH pH, std::string version)
{
    QString qVersion = QString::fromStdString(version);
    qDebug() << "CDO Initialized, version: " << qVersion <<
                ". Continuing with initialization";
    emit cdoReady(pH, qVersion);

    CDODevsHandler rH = boost::bind(&AppController::onVideoDevices, this, _1,
                                    true);
    _cdoCtrl.getVideoCaptureDeviceNames(rH);

    rH = boost::bind(&AppController::onAudioCaptureDevices, this, _1, true);
    _cdoCtrl.getAudioCaptureDeviceNames(rH);

    rH = boost::bind(&AppController::onAudioOutputDevices, this, _1, true);
    _cdoCtrl.getAudioOutputDeviceNames(rH);
}

void AppController::onVideoDevices(std::map<std::string,std::string> devs,
                                   bool firstRun)
{
    qDebug() << "Got video devices list containing " << devs.size() << " items";
    emit mediaDevicesListChanged(VIDEO_IN, devsMap2QVariantMap(devs));
    if(firstRun)
    {
        qDebug() << "Setting video capture device";
        CDOSetDevHandler rh =
                boost::bind(&AppController::onVideoDeviceSet, this, firstRun);
        _cdoCtrl.setVideoCaptureDevice(rh, devs.begin()->first);
    }
}

void AppController::onAudioCaptureDevices(
        std::map<std::string,std::string> devs,bool firstRun)
{
    qDebug() << "Got video devices list containing " << devs.size() << " items";
    emit mediaDevicesListChanged(AUDIO_IN, devsMap2QVariantMap(devs));
    if(firstRun)
    {
        qDebug() << "Setting audio capture device";
        _cdoCtrl.setAudioCaptureDevice(&nop, devs.begin()->first);
    }
}

void AppController::onAudioOutputDevices(
        std::map<std::string,std::string> devs, bool firstRun)
{
    qDebug() << "Got video devices list containing " << devs.size() << " items";
    emit mediaDevicesListChanged(AUDIO_OUT, devsMap2QVariantMap(devs));
    if(firstRun)
    {
        qDebug() << "Setting audio output device";
        _cdoCtrl.setAudioOutputDevice(&nop, devs.begin()->first);
    }
}

void AppController::onVideoDeviceSet(bool startLocalVideo)
{
    if(!startLocalVideo)
        return;
    qDebug() << "Video device configured; Starting local preview";
    CDOLocalVideoStartedHandler rH =
            boost::bind(&AppController::onLocalVideoStarted, this, _1);
    _cdoCtrl.startLocalVideo(rH);
}

void AppController::onLocalVideoStarted(std::string sinkId)
{
    QString qSinkId = QString::fromStdString(sinkId);
    qDebug() << "Local preview started; Sink id: " << qSinkId;
    emit localVideoSinkChanged(qSinkId);
}

namespace
{
QVariantMap devsMap2QVariantMap(const std::map<std::string,std::string> devs)
{
    QVariantMap qDevs;
    std::pair<std::string, std::string> itm;
    BOOST_FOREACH(itm, devs)
    {
        qDevs[QString::fromStdString(itm.first)] =
                QString::fromStdString(itm.second);
    }
    return qDevs;

}
}
