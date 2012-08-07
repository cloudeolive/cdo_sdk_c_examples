#include <cloudeoctrl.h>

#include <cdohelpers.h>

#include <string>
#include <string.h>
#include <windows.h>
#define CLOUDEO_SDK_HOME "cloudeo_sdk"

const std::string gLibsPath = CLOUDEO_SDK_HOME;


CloudeoCtrl::CloudeoCtrl(MainWindow* window): _window(window)
{
    char exePath[2048];
    GetModuleFileNameA(NULL,exePath,2048);
    int pathSize = strlen(exePath);
    int dirPathSize = pathSize - strlen("cdo_sample_app.exe");
    std::string sdkPath(exePath, dirPathSize);
    sdkPath += gLibsPath;

    CDOInitOptions options;
    CDOHelpers::stdString2CdoString(&options.logicLibPath, sdkPath);
    cdo_init_platform(&CloudeoCtrl::onPlatformReady,&options,this);
}

void CloudeoCtrl::onPlatformReady(void* o, const CDOError* err, CDOH h)
{
    CloudeoCtrl* ctrl = (CloudeoCtrl*)o;
    if(cdo_no_error(err))
    {
        ctrl->_platformHandle = h;
        cdo_get_version(&CloudeoCtrl::onVersion,h, ctrl);
        cdo_get_video_capture_device_names(&CloudeoCtrl::onDevices,h, ctrl);
    }
    else
    {
        ctrl->_window->setVersion("Failed to init platform");
    }

}

void CloudeoCtrl::onVersion(void* o, const CDOError* e, const CDOString* v)
{
    CloudeoCtrl* ctrl = (CloudeoCtrl*) o;
    ctrl->_window->setVersion(std::string(v->body, v->length));
}


void CloudeoCtrl::onDevices(void* o, const CDOError* e,
                            CDODevice* devs, size_t len)
{
    CloudeoCtrl* ctrl = (CloudeoCtrl*) o;
    cdo_set_video_capture_device(&CloudeoCtrl::onSetVideoCaptureDevice,
                                 ctrl->_platformHandle,ctrl,&(devs[0].id));
}

void CloudeoCtrl::onSetVideoCaptureDevice(void* o, const CDOError* e)
{
    CloudeoCtrl* ctrl = (CloudeoCtrl*) o;
    cdo_start_local_video(&CloudeoCtrl::onLocalPreviewStarted,ctrl->
                          _platformHandle,ctrl);
}


void CloudeoCtrl::onLocalPreviewStarted(void* o, const CDOError* e,
                                        const CDOString* v)
{
    CloudeoCtrl* ctrl = (CloudeoCtrl*) o;
    ctrl->_window->startRendering(ctrl->_platformHandle,
                                  std::string(v->body, v->length));
}
