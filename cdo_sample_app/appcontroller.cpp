#include "appcontroller.h"

#include <boost/bind.hpp>
#include <boost/foreach.hpp>
#include <QDebug>
#include <QTime>
#include <cdohelpers.h>
#include <boost/lexical_cast.hpp>


namespace
{
QVariantMap devsMap2QVariantMap(const std::map<std::string,std::string> in);
void nop(){}

// Dev layer streamer
const std::string gStreamerBase = "46.137.126.193:7005/";
}

AppController::AppController(QObject *parent) :
    QObject(parent), _connected(false)
{
}

void AppController::initCDO()
{
    qDebug() << "Initializing CDO";
    CDOReadyHandler rH = boost::bind(&AppController::onCdoReady, this, _1, _2);
    _cdoCtrl.initPlatform(rH);
}

void AppController::connect(QString scopeId, bool pAudio, bool pVideo)
{
    qDebug() << "Establishing a connection to scope with id: " << scopeId;
    CDOConnectionDescriptor descr;
    descr.autopublishAudio = pAudio;
    descr.autopublishVideo = pVideo;
    QTime m(0,0,0);
    qsrand(m.secsTo(QTime::currentTime()));
    int uId = qrand() % 1000;
    descr.token = CDOHelpers::stdString2CdoString(
                boost::lexical_cast<std::string>(uId));
    descr.url = CDOHelpers::stdString2CdoString(
                gStreamerBase + scopeId.toStdString());

    descr.lowVideoStream.maxBitRate = 64;
    descr.lowVideoStream.maxFps = 5;
    descr.lowVideoStream.maxHeight = 240;
    descr.lowVideoStream.maxWidth = 320;

    descr.highVideoStream.maxBitRate = 512;
    descr.highVideoStream.maxWidth = 640;
    descr.highVideoStream.maxHeight = 480;
    descr.highVideoStream.maxFps = 24;
    _scopeId = scopeId.toStdString();
    CDOConnectedHandler rh = boost::bind(&AppController::onConnected, this, _1);
    _cdoCtrl.connect(rh, &descr);
}

void AppController::onUserEvent(void* opaque, const CDOUserStateChangedEvent* e)
{
    ((AppController*) opaque)->onUserEvent(e);

}

void AppController::onMediaEvent(void* opaque,
                                 const CDOUserStateChangedEvent* e)
{
    ((AppController*) opaque)->onMediaEvent(e);
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

void AppController::disconnectBtnClicked()
{
    qDebug() << "Terminating connection";
    _cdoCtrl.disconnect(_scopeId);
    _connected = false;
}


void AppController::videoPublishStateChanged(bool state)
{
    if(_connected)
    {
        if(state)
        {
            qDebug() << "Publishing video";
            _cdoCtrl.publish(_scopeId, CDO_MEDIA_TYPE_VIDEO);
        }
        else
        {
            qDebug() << "Unpublishing video";
            _cdoCtrl.unpublish(_scopeId, CDO_MEDIA_TYPE_VIDEO);
        }
    }
}

void AppController::audioPublishStateChanged(bool state)
{
    if(_connected)
    {
        if(state)
        {
            qDebug() << "Publishing audio";
            _cdoCtrl.publish(_scopeId, CDO_MEDIA_TYPE_AUDIO);
        }
        else
        {
            qDebug() << "Unpublishing audio";
            _cdoCtrl.unpublish(_scopeId, CDO_MEDIA_TYPE_AUDIO);
        }
    }
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

    CDOServiceListener listener;
    memset(&listener,0,sizeof(listener));
    listener.onUserEvent = &AppController::onUserEvent;
    listener.onMediaStreamEvent = &AppController::onMediaEvent;
    listener.opaque = this;
    _cdoCtrl.addPlatformListener(&listener);

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

void AppController::onConnected(bool succ)
{
    qDebug() << "Got connected result: " << succ;
    _connected = true;
}


void AppController::onUserEvent(const CDOUserStateChangedEvent* e)
{
    qDebug() << "Got new user event";
    if(e->isConnected && e->videoPublished)
        emit remoteVideoSinkChanged(
                    CDOHelpers::cdoString2QString(&(e->videoSinkId)));
    else
        emit remoteVideoSinkChanged(QString());

}

void AppController::onMediaEvent(const CDOUserStateChangedEvent* e)
{
    qDebug() << "Got new media event, related to media type: " <<
                CDOHelpers::cdoString2QString(&(e->mediaType));
    if(CDOHelpers::stringEq(&(e->mediaType), CDO_MEDIA_TYPE_VIDEO))
    {
        QString sinkId = e->videoPublished ?
                    CDOHelpers::cdoString2QString(&(e->videoSinkId)) :
                    QString();
        emit remoteVideoSinkChanged(sinkId);
    }
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
