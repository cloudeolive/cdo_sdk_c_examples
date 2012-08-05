#ifndef CLOUDEOCTRL_H
#define CLOUDEOCTRL_H

#include <cloudeo_sdk.h>

#include <boost/function.hpp>

#include <map>
#include <string>

typedef boost::function<void(CDOH,std::string)> CDOReadyHandler;
typedef boost::function<void(std::map<std::string,std::string>)> CDODevsHandler;
typedef boost::function<void()> CDOSetDevHandler;
typedef boost::function<void(std::string)> CDOLocalVideoStartedHandler;

class CloudeoCtrl
{
public:
    CloudeoCtrl();

    void initPlatform(CDOReadyHandler readyHandler);

    void getVideoCaptureDeviceNames(CDODevsHandler);
    void getAudioCaptureDeviceNames(CDODevsHandler);
    void getAudioOutputDeviceNames(CDODevsHandler);

    void setVideoCaptureDevice(CDOSetDevHandler rH, std::string);
    void setAudioCaptureDevice(CDOSetDevHandler rH, std::string);
    void setAudioOutputDevice(CDOSetDevHandler rH, std::string);
    void playTestSound();

    void startLocalVideo(CDOLocalVideoStartedHandler rH);

    static void onPlatformReady(void* o, const CDOError* err, CDOH h);

    static void onVersion(void* o, const CDOError* e, const CDOString* v);

    static void onDevices(void* o, const CDOError* e, CDODevice* devs, size_t len);

    static void onSetDevice(void* o, const CDOError* e);

    static void onLocalPreviewStarted(void* o, const CDOError* e, const CDOString* v);

private:


    static void nopRHandler(void* o, const CDOError* e){}

    CDOH _platformHandle;
    CDOReadyHandler _readyHandler;
};

#endif // CLOUDEOCTRL_H
