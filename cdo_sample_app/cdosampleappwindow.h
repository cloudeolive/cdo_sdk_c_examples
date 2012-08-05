#ifndef CDOSAMPLEAPPWINDOW_H
#define CDOSAMPLEAPPWINDOW_H

#include <QMainWindow>
#include <appcontroller.h>
#include <QMap>
#include <QVariant>
#include <QString>
#include <cloudeo_sdk.h>
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
    void onCDOPlatformReady(void*, QString);
signals:

private:

    void setupBindings();

    Ui::CdoSampleAppWindow *ui;

    AppController _appController;
};

#endif // CDOSAMPLEAPPWINDOW_H
