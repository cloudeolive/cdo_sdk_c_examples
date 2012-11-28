#ifndef APPCONTROLLER_H
#define APPCONTROLLER_H

#include <cloudeoctrl.h>
#include <addlive_sdk.h>
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

    void initADL();

    void connect(QString scopeId, bool pAudio, bool pVideo);


//  Callbacks
    void onCdoReady(ADLH, std::string v);
    void onVideoDevices(std::map<std::string,std::string> devs,bool);
    void onAudioCaptureDevices(std::map<std::string,std::string> devs,bool);
    void onAudioOutputDevices(std::map<std::string,std::string> devs,bool);
    void onVideoDeviceSet(bool startLocalVideo);
    void onLocalVideoStarted(std::string sinkId);
    void onConnected(bool succ);

    static void onUserEvent(void* opaque, const ADLUserStateChangedEvent*);
    static void onMediaEvent(void* opaque, const ADLUserStateChangedEvent*);
signals:

    void cdoReady(void* ph, QString v);
    
    void mediaDevicesListChanged(int,QVariantMap);

    void localVideoSinkChanged(QString);
    void remoteVideoSinkChanged(QString);
    void connected();
public slots:

    void disconnectBtnClicked();
    void playTestSndClicked();
    void videoPublishStateChanged(bool state);
    void audioPublishStateChanged(bool state);


private:

    void onUserEvent(const ADLUserStateChangedEvent*);
    void onMediaEvent(const ADLUserStateChangedEvent*);


    CloudeoCtrl _cdoCtrl;
    bool _connected;
    std::string _scopeId;
};

#endif // APPCONTROLLER_H
