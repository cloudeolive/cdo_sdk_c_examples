#include "renderingwidget.h"
#include <string.h>
#include <cdohelpers.h>
#include <QPaintEvent>
#include <QPaintEngine>
#include <QPainter>
#include <iostream>
#include <Windows.h>
RenderingWidget::RenderingWidget(QWidget *parent) :
    QWidget(parent), _started(false)
{
    setAttribute(Qt::WA_PaintOnScreen, true);
}


void RenderingWidget::startRender(std::string sinkId)
{
    if(_started)
    {
        _sinkIdToBeRendered = sinkId;
        stopRender();
    }
    CDORenderRequest rRequest;
    CDOHelpers::stdString2CdoString(&rRequest.sinkId, sinkId);
    CDOHelpers::stdString2CdoString(&rRequest.filter, "fast_bilinear");
    rRequest.mirror = true; rRequest.invalidateCallback =
    &RenderingWidget::invalidateClbck; rRequest.opaque = this;
    rRequest.windowHandle = NULL;
    cdo_render_sink(&RenderingWidget::renderStarted,_platformHandle,this,
                    &rRequest);
    setUpdatesEnabled(true);
}

void RenderingWidget::stopRender()
{
    cdo_stop_render(&RenderingWidget::renderStopped, _platformHandle, this,
                    _rendererId);
}

void RenderingWidget::setPlatformHandle(CDOH handle)
{
    _platformHandle = handle;
}


void RenderingWidget::paintEvent(QPaintEvent *e)
{
    if(_started)
    {
        CDODrawRequest req;

        QRect r;
        if(e)
            r = e->rect();
        else
            r = rect();
        QRect geo = geometry();
        QRect frameGeo = frameGeometry();
        QPoint global = mapTo(nativeParentWidget(), QPoint(0,0));
        req.left = global.x();
        req.right = req.left + width();
        req.top = global.y();
        req.bottom = req.top + height();
        req.rendererId = _rendererId;

        QPainter painter(this);
        HDC hdc = painter.paintEngine()->getDC();
        req.windowHandle = hdc;

        cdo_draw(_platformHandle, &req);
    }
    else
    {
        QPainter painter(this);
        painter.setBackground(
                    QBrush(QColor::fromRgb(120,120,120)));
        painter.drawText(QRectF(),QString("Waiting for frame.,."));
    }
}


void RenderingWidget::invalidateClbck(void* o)
{
    ((RenderingWidget*)o)->invalidateClbckImpl();
}

void RenderingWidget::renderStarted(void* o, const CDOError* err,
                                    int rendererId)
{
    RenderingWidget* _this = (RenderingWidget*) o;
    _this->_rendererId = rendererId;
    _this->_started = true;
}

void RenderingWidget::renderStopped(void* o , const CDOError* err)
{
    RenderingWidget* _this = (RenderingWidget*) o;
    _this->sinkId = "";
    _this->_started = false;
    if(_this->_sinkIdToBeRendered.length() > 0)
    {
        _this->startRender(_this->_sinkIdToBeRendered);
    }
}

void RenderingWidget::invalidateClbckImpl()
{
    this->update();
}
