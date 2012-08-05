#ifndef RENDERINGWIDGET_H
#define RENDERINGWIDGET_H

#include <QWidget>
#include <string>
#include <cloudeo_sdk.h>

class RenderingWidget : public QWidget
{
    Q_OBJECT
public:
    explicit RenderingWidget(QWidget *parent = 0);
    
    void startRender(std::string sinkId);

    void stopRender();

    void setPlatformHandle(CDOH handle);

    static void invalidateClbck(void*);

    static void renderStarted(void*, const CDOError*, int);
    static void renderStopped(void*, const CDOError*);

protected:
    void paintEvent(QPaintEvent *e);


signals:
    
public slots:
    
private:

    void invalidateClbckImpl();

    CDOH _platformHandle;
    std::string sinkId;
    std::string _sinkIdToBeRendered;
    int _rendererId;
    bool _started;
};

#endif // RENDERINGWIDGET_H
