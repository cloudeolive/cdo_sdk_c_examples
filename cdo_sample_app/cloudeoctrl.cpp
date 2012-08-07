#include <cloudeoctrl.h>

#include <cdohelpers.h>

#include <string>
#include <string.h>
#include <windows.h>
#include <QDebug>


#define CLOUDEO_SDK_HOME "cloudeo_sdk"

const std::string gLibsPath = CLOUDEO_SDK_HOME;


CloudeoCtrl::CloudeoCtrl()
{
}

void CloudeoCtrl::initPlatform(CDOReadyHandler readyHandler)
{
    char exePath[2048];
    GetModuleFileNameA(NULL,exePath,2048);
    int pathSize = strlen(exePath);
    int dirPathSize = pathSize - strlen("cdo_sample_app.exe");
    std::string sdkPath(exePath, dirPathSize);
    sdkPath += gLibsPath;
    CDOInitOptions options;
    CDOHelpers::stdString2CdoString(&options.logicLibPath, sdkPath );
    _readyHandler = readyHandler;
    cdo_init_platform(&CloudeoCtrl::onPlatformReady,&options,this);
}

void nopRHandler(void* o, const CDOError* e)
{
}

void CloudeoCtrl::addPlatformListener(CDOServiceListener* listener)
{
    cdo_add_service_listener(&nopRHandler,_platformHandle, this, listener);
}



void CloudeoCtrl::getVideoCaptureDeviceNames(CDODevsHandler resultHandler)
{
    CDODevsHandler * copy = new CDODevsHandler();
    memcpy(copy, &resultHandler, sizeof(*copy));
    cdo_get_video_capture_device_names(&CloudeoCtrl::onDevices,_platformHandle,
                                       copy);
}

void CloudeoCtrl::getAudioCaptureDeviceNames(CDODevsHandler  resultHandler)
{
    CDODevsHandler * copy = new CDODevsHandler();
    memcpy(copy, &resultHandler, sizeof(*copy));
    cdo_get_audio_capture_device_names(&CloudeoCtrl::onDevices,_platformHandle,
                                       copy);
}

void CloudeoCtrl::getAudioOutputDeviceNames(CDODevsHandler resultHandler)
{
    CDODevsHandler * copy = new CDODevsHandler();
    memcpy(copy, &resultHandler, sizeof(*copy));
    cdo_get_audio_output_device_names(&CloudeoCtrl::onDevices,_platformHandle,
                                       copy);
}

void CloudeoCtrl::setVideoCaptureDevice(CDOSetDevHandler rH, std::string dev)
{
    CDOString devId = CDOHelpers::stdString2CdoString(dev);
    CDOSetDevHandler * copy = new CDOSetDevHandler();
    memcpy(copy, &rH, sizeof(*copy));

    cdo_set_video_capture_device(&CloudeoCtrl::onSetDevice, _platformHandle,
                                 copy, &devId);
}

void CloudeoCtrl::setAudioCaptureDevice(CDOSetDevHandler rH,std::string dev)
{
    CDOString devId = CDOHelpers::stdString2CdoString(dev);
    CDOSetDevHandler * copy = new CDOSetDevHandler();
    memcpy(copy, &rH, sizeof(*copy));
    cdo_set_audio_capture_device(&CloudeoCtrl::onSetDevice, _platformHandle,
                                 copy, &devId);
}

void CloudeoCtrl::setAudioOutputDevice(CDOSetDevHandler rH, std::string dev)
{
    CDOString devId = CDOHelpers::stdString2CdoString(dev);
    CDOSetDevHandler * copy = new CDOSetDevHandler();
    memcpy(copy, &rH, sizeof(*copy));
    cdo_set_audio_output_device(&CloudeoCtrl::onSetDevice, _platformHandle,
                                 copy, &devId);
}

void CloudeoCtrl::playTestSound()
{
    cdo_start_playing_test_sound(&CloudeoCtrl::nopRHandler, _platformHandle,
                                 this);
}

void CloudeoCtrl::startLocalVideo(CDOLocalVideoStartedHandler rH)
{
    CDOLocalVideoStartedHandler * copy = new CDOLocalVideoStartedHandler();
    memcpy(copy, &rH, sizeof(*copy));
    cdo_start_local_video(&CloudeoCtrl::onLocalPreviewStarted, _platformHandle,
                          copy);

}


void CloudeoCtrl::connect(CDOConnectedHandler rH,
                          CDOConnectionDescriptor* descr)
{
    CDOConnectedHandler* copy = new CDOConnectedHandler();
    memcpy(copy, &rH, sizeof(*copy));
    cdo_connect(&CloudeoCtrl::onConnected, _platformHandle, copy, descr);
}

void CloudeoCtrl::disconnect(std::string scopeId)
{
    CDOString cdoScopeId = CDOHelpers::stdString2CdoString(scopeId);
    cdo_disconnect(&CloudeoCtrl::nopRHandler, _platformHandle, this,
                   &cdoScopeId);
}


void CloudeoCtrl::publish(std::string scopeId, std::string what)
{
    CDOString cdoScopeId = CDOHelpers::stdString2CdoString(scopeId);
    CDOString cdoWhat = CDOHelpers::stdString2CdoString(what);
    cdo_publish(&CloudeoCtrl::nopRHandler,_platformHandle,0,
                &cdoScopeId, &cdoWhat);
}

void CloudeoCtrl::unpublish(std::string scopeId, std::string what)
{
    CDOString cdoScopeId = CDOHelpers::stdString2CdoString(scopeId);
    CDOString cdoWhat = CDOHelpers::stdString2CdoString(what);

    cdo_unpublish(&CloudeoCtrl::nopRHandler,_platformHandle,0,
                &cdoScopeId, &cdoWhat);

}


void CloudeoCtrl::onPlatformReady(void* o, const CDOError* err, CDOH h)
{
    CloudeoCtrl* ctrl = (CloudeoCtrl*)o;
    if(cdo_no_error(err))
    {
        ctrl->_platformHandle = h;
        cdo_get_version(&CloudeoCtrl::onVersion,h, ctrl);
    }
    else
    {
        QString errorStr =
                QString::fromAscii(err->err_message.body,
                                   err->err_message.length );
        qWarning() << "Failed to initialize the Cloudeo platform, due to: "
                 << errorStr;
    }
}

void CloudeoCtrl::onVersion(void* o, const CDOError* e, const CDOString* v)
{
    CloudeoCtrl* ctrl = (CloudeoCtrl*) o;
    try
    {
        ctrl->_readyHandler(ctrl->_platformHandle, std::string(v->body, v->length));
    } catch(const std::exception& exception)
    {
        qDebug() << "Got an exxception: " << exception.what();
    }
}


void CloudeoCtrl::onDevices(void* o, const CDOError* e,
                            CDODevice* devs, size_t len)
{
    CDODevsHandler* rH = (CDODevsHandler*) o;
    std::map<std::string,std::string> result;
    for(int i=0;i<len;i++)
    {
        std::string id = CDOHelpers::cdoString2Std(&(devs[i].id));
        std::string label = CDOHelpers::cdoString2Std(&(devs[i].label));
        result[id] = label;
    }
    (*rH)(result);
    delete rH;
}

void CloudeoCtrl::onSetDevice(void* o, const CDOError* e)
{
    CDOSetDevHandler* rH = (CDOSetDevHandler*) o;
    (*rH)();
    delete rH;
}

void CloudeoCtrl::onLocalPreviewStarted(void* o, const CDOError* e,
                                        const CDOString* v)
{
    CDOLocalVideoStartedHandler* rH = (CDOLocalVideoStartedHandler*) o;
    (*rH)(CDOHelpers::cdoString2Std(v));
    delete rH;
}

void CloudeoCtrl::onConnected(void* o, const CDOError* e)
{
    CDOConnectedHandler* rH = (CDOConnectedHandler*)o;
    (*rH)(cdo_no_error(e));
    delete rH;

}
