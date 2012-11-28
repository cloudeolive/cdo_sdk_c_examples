#ifndef CLOUDEOCTRL_H
#define CLOUDEOCTRL_H

#include <addlive_sdk.h>

#include <boost/function.hpp>

#include <map>
#include <string>

typedef boost::function<void(ADLH,std::string)> ADLReadyHandler;
typedef boost::function<void(std::map<std::string,std::string>)> ADLDevsHandler;
typedef boost::function<void()> ADLSetDevHandler;
typedef boost::function<void(std::string)> ADLLocalVideoStartedHandler;
typedef boost::function<void(bool)> ADLConnectedHandler;

class CloudeoCtrl
{
public:
    CloudeoCtrl();

    void initPlatform(ADLReadyHandler readyHandler);

    void addPlatformListener(ADLServiceListener* listener);

    void getVideoCaptureDeviceNames(ADLDevsHandler);
    void getAudioCaptureDeviceNames(ADLDevsHandler);
    void getAudioOutputDeviceNames(ADLDevsHandler);

    void setVideoCaptureDevice(ADLSetDevHandler rH, std::string);
    void setAudioCaptureDevice(ADLSetDevHandler rH, std::string);
    void setAudioOutputDevice(ADLSetDevHandler rH, std::string);
    void playTestSound();

    void startLocalVideo(ADLLocalVideoStartedHandler rH);

    void connect(ADLConnectedHandler rh, ADLConnectionDescriptor* descr, std::string scopeId);
    void disconnect(std::string scopeId);

    void publish(std::string scopeId, std::string what);
    void unpublish(std::string scopeId, std::string what);

    static void onPlatformReady(void* o, const ADLError* err, ADLH h);

    static void onVersion(void* o, const ADLError* e, const ADLString* v);
    static void onAppIdSet(void* o, const ADLError* e);

    static void onDevices(void* o, const ADLError* e, ADLDevice* devs,
                          size_t len);

    static void onSetDevice(void* o, const ADLError* e);

    static void onLocalPreviewStarted(void* o, const ADLError* e,
                                      const ADLString* v);
    static void onConnected(void* o, const ADLError* e);

private:


    static void nopRHandler(void* o, const ADLError* e){}

    static const long long APP_ID = 2;
    static const std::string APP_SECRET;

    ADLH _platformHandle;
    ADLReadyHandler _readyHandler;
};

#endif // CLOUDEOCTRL_H
