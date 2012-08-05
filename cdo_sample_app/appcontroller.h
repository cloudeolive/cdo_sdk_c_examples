#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <cloudeoctrl.h>
#include <cloudeo_sdk.h>
#include <QObject>
#include <QVariant>
#include <QMap>
#include <map>
#include <string>

enum MediaDeviceType
{
    AUDIO_IN, AUDIO_OUT, VIDEO_IN
};

class AppController : public QObject
{
    Q_OBJECT
public:

    explicit AppController(QObject *parent = 0);

    void initCDO();

    void connect(QString scopeId, bool pAudio, bool pVideo);


//  Callbacks
    void onCdoReady(CDOH, std::string v);
    void onVideoDevices(std::map<std::string,std::string> devs,bool);
    void onAudioCaptureDevices(std::map<std::string,std::string> devs,bool);
    void onAudioOutputDevices(std::map<std::string,std::string> devs,bool);
    void onVideoDeviceSet(bool startLocalVideo);
    void onLocalVideoStarted(std::string sinkId);
    void onConnected(bool succ);

    static void onUserEvent(void* opaque, const CDOUserStateChangedEvent*);
    static void onMediaEvent(void* opaque, const CDOUserStateChangedEvent*);
signals:

    void cdoReady(void* ph, QString v);
    
    void mediaDevicesListChanged(int,QVariantMap);

    void localVideoSinkChanged(QString);
    void remoteVideoSinkChanged(QString);
public slots:

    void disconnectBtnClicked();
    void playTestSndClicked();
    void videoPublishStateChanged(bool state);
    void audioPublishStateChanged(bool state);


private:

    void onUserEvent(const CDOUserStateChangedEvent*);
    void onMediaEvent(const CDOUserStateChangedEvent*);


    CloudeoCtrl _cdoCtrl;
    bool _connected;
    std::string _scopeId;
};

#endif // APPCONTROLLER_H
