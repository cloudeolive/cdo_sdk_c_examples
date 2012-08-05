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

//  Callbacks
    void onCdoReady(CDOH, std::string v);
    void onVideoDevices(std::map<std::string,std::string> devs,bool);
    void onAudioCaptureDevices(std::map<std::string,std::string> devs,bool);
    void onAudioOutputDevices(std::map<std::string,std::string> devs,bool);
    void onVideoDeviceSet(bool startLocalVideo);
    void onLocalVideoStarted(std::string sinkId);
signals:

    void cdoReady(void* ph, QString v);
    
    void mediaDevicesListChanged(int,QVariantMap);

    void localVideoSinkChanged(QString);
public slots:

    void playTestSndClicked();


private:

    CloudeoCtrl _cdoCtrl;
    
};

#endif // APPCONTROLLER_H
