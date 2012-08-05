#ifndef CLOUDEOCTRL_H
#define CLOUDEOCTRL_H
#include <mainwindow.h>
#include <cloudeo_sdk.h>

class CloudeoCtrl
{
public:
    CloudeoCtrl(MainWindow* window);

    static void onPlatformReady(void* o, const CDOError* err, CDOH h);

    static void onVersion(void* o, const CDOError* e, const CDOString* v);

    static void onDevices(void* o, const CDOError* e, CDODevice* devs, size_t len);
    static void onSetVideoCaptureDevice(void* o, const CDOError* e);

    static void onLocalPreviewStarted(void* o, const CDOError* e, const CDOString* v);
private:
    MainWindow* _window;
    CDOH _platformHandle;
};

#endif // CLOUDEOCTRL_H
