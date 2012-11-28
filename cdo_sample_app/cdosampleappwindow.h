#ifndef ADLSAMPLEAPPWINDOW_H
#define ADLSAMPLEAPPWINDOW_H

#include <QMainWindow>
#include <appcontroller.h>
#include <QMap>
#include <QVariant>
#include <QString>
#include <addlive_sdk.h>
namespace Ui {
class CdoSampleAppWindow;
}

class CdoSampleAppWindow : public QMainWindow
{
    Q_OBJECT
    
public:
    explicit CdoSampleAppWindow(QWidget *parent = 0);
    ~CdoSampleAppWindow();

public slots:

    void onMediaDevicesListChanged(int, QVariantMap);
    void onLocalPreviewSinkChanged(QString);
    void onRemotePreviewSinkChanged(QString);
    void onADLPlatformReady(void*, QString);
    void onConnectClicked();
    void onConnected();
signals:

private:

    void setupBindings();

    Ui::CdoSampleAppWindow *ui;

    AppController _appController;
};

#endif // ADLSAMPLEAPPWINDOW_H
