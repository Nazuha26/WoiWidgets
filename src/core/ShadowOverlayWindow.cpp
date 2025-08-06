//********************************************************************
//                                                                   *
//         Project : WoiWidgets                                      *
//         File    : ShadowOverlayWindow.cpp                         *
//         Author  : Nazuha26                                        *
//                                                                   *
//         Copyright (c) 2025 by Nazuha26                            *
//         All bugs are my own. Handle with care.                    *
//         Copyright last updated on 03.08.2025 at (19:34:11)        *
//                                                                   *
//********************************************************************

//         Created by Nazuha26 in CLion
//         Date: 03.08.2025
//         Time: 19:34:11

#include "ShadowOverlayWindow.h"

#include <QEvent>
#include <QPainter>

#include "style/WoiThemeEngine.h"

#ifdef Q_OS_WIN
#include <windows.h>
#endif



ShadowOverlayWindow::ShadowOverlayWindow(QWidget* parent) : WoiThemeWidget(parent),
                                                            offsetX_(0),
                                                            offsetY_(0),
                                                            margin_(16),
                                                            softness_(3),

                                                            /* local */
                                                            targetWindow_(nullptr)
{
    setWindowFlags(Qt::Tool
              | Qt::FramelessWindowHint
              | Qt::WindowDoesNotAcceptFocus
              | Qt::WindowTransparentForInput);

    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_NoSystemBackground);
    setAttribute(Qt::WA_TranslucentBackground);
    setFocusPolicy(Qt::NoFocus);

    syncStyle();   // synchronize shadow window style with the theme

    setWindowTitle("shadow");
    show();
}



void ShadowOverlayWindow::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter p(this);
    //p.setRenderHint(QPainter::Antialiasing);
    p.setPen(Qt::NoPen);


    const QRect r            = rect();
    const int   left         = r.left();
    const int   right        = r.x() + r.width();
    const int   top          = r.top();
    const int   bottom       = r.y() + r.height();
    const int   calcSoftness = qRound(margin_ * softness_);



    p.fillRect(r.adjusted(calcSoftness, calcSoftness, -calcSoftness, -calcSoftness), color_);


    // ----- LEFT -----
    drawSideGradient(p, QRect(left, top + calcSoftness, calcSoftness, r.height() - 2 * calcSoftness), Qt::Horizontal,
                     color_);

    // ----- RIGHT -----
    drawSideGradient(p, QRect(right - calcSoftness, top + calcSoftness, calcSoftness, r.height() - 2 * calcSoftness),
                     Qt::Horizontal, color_, true);

    // ----- TOP -----
    drawSideGradient(p, QRect(left + calcSoftness, top, r.width() - 2 * calcSoftness, calcSoftness), Qt::Vertical,
                     color_);

    // ----- BOTTOM -----
    drawSideGradient(p, QRect(left + calcSoftness, bottom - calcSoftness, r.width() - 2 * calcSoftness, calcSoftness),
                     Qt::Vertical, color_, true);



    // ----- TOP-LEFT CORNER -----
    drawCornerGradient(p, QRect(left, top, calcSoftness, calcSoftness),
                       QPointF(left + calcSoftness, top + calcSoftness), color_);

    // ----- TOP-RIGHT CORNER -----
    drawCornerGradient(p, QRect(right - calcSoftness, top, calcSoftness, calcSoftness),
                       QPointF(right - calcSoftness, top + calcSoftness), color_);

    // ----- BOTTOM-LEFT CORNER -----
    drawCornerGradient(p, QRect(left, bottom - calcSoftness, calcSoftness, calcSoftness),
                       QPointF(left + calcSoftness, bottom - calcSoftness), color_);

    // ----- BOTTOM-RIGHT CORNER -----
    drawCornerGradient(p, QRect(right - calcSoftness, bottom - calcSoftness, calcSoftness, calcSoftness),
                       QPointF(right - calcSoftness, bottom - calcSoftness), color_);



    /* ----- GLOBAL MASK GRADIENT ----- */
    {
        QLinearGradient globalMaskGrad(0, top, 0, bottom);

        globalMaskGrad.setColorAt(0, QColor(0, 0, 0, static_cast<int>(topAlpha_ * 0xFF)));
        globalMaskGrad.setColorAt(1, QColor(0, 0, 0, static_cast<int>(bottomAlpha_ * 0xFF)));

        const QPainter::CompositionMode compositionMode = p.compositionMode();
        p.setCompositionMode(QPainter::CompositionMode_DestinationIn); // mask by alpha
        p.fillRect(rect(), globalMaskGrad);
        p.setCompositionMode(compositionMode); // return default composition mode
    }
}



bool ShadowOverlayWindow::eventFilter(QObject* watched, QEvent* event)
{
    if (watched == targetWindow_)
    {
        switch (event->type())
        {
            case QEvent::Move:
            case QEvent::Resize:
            case QEvent::Show: syncWithTarget();
                break;
            case QEvent::Hide: hide();
                break;
            case QEvent::WindowActivate: syncZOrder();
                break;
            case QEvent::WindowStateChange: {
                const Qt::WindowStates state = targetWindow_->windowState();
                if (state & Qt::WindowMinimized || state & Qt::WindowMaximized)
                    hide();
                else
                {
                    show();
                    syncWithTarget();
                }
                break;
            }
            default: break;
        }
    }

    return QWidget::eventFilter(watched, event);
}


/* ----- THEME MANAGMENT ----- */
void ShadowOverlayWindow::applyTheme(const WoiTheme& theme)
{ applyStyle(theme.shadowOverlay); }



void ShadowOverlayWindow::applyStyle(const WoiTheme::ShadowOverlayWindowStyle& style)
{
    color_ = style.color;
    topAlpha_ = style.topAlpha;
    bottomAlpha_ = style.bottomAlpha;

    update();
}



void ShadowOverlayWindow::syncStyle()
{ applyStyle(ThemeEngine->currentTheme().shadowOverlay); }
/* ----- THEME MANAGMENT ----- */



void ShadowOverlayWindow::setTargetWindow(QWidget* target)
{
    targetWindow_ = target;
    targetWindow_->installEventFilter(this);
    syncWithTarget();
}



/* ----- CORE ----- */
void ShadowOverlayWindow::syncWithTarget()
{
    if (!targetWindow_)
        return;
    const QRect targetGeom = targetWindow_->frameGeometry();
    move(targetGeom.topLeft() - QPoint(margin_, margin_) + QPoint(offsetX_, offsetY_));
    resize(targetGeom.size() + QSize(margin_ * 2, margin_ * 2));
    syncZOrder();
}



void ShadowOverlayWindow::syncZOrder() const
{
#ifdef Q_OS_WIN
    const HWND hwndShadow = reinterpret_cast<HWND>(winId());
    const HWND hwndParent = reinterpret_cast<HWND>(targetWindow_->winId());

    SetWindowPos(hwndShadow, hwndParent, 0, 0, 0, 0, SWP_NOMOVE | SWP_NOSIZE | SWP_NOACTIVATE);
#endif
}



void ShadowOverlayWindow::drawSideGradient(QPainter&     p, const QRect& rect, const Qt::Orientation orientation,
                                           const QColor& color, bool     reverse)
{
    QLinearGradient gradient;
    const int left   = rect.left();
    const int right  = rect.x() + rect.width();
    const int top    = rect.top();
    const int bottom = rect.y() + rect.height();
    if (orientation == Qt::Horizontal)
    {
        if (reverse)
            gradient = QLinearGradient(right, top, left, top); // Right to Left
        else
            gradient = QLinearGradient(left, top, right, top); // Left to Right
    }
    else
    {
        if (reverse)
            gradient = QLinearGradient(left, bottom, left, top); // Bottom to Top
        else
            gradient = QLinearGradient(left, top, left, bottom); // Top to Bottom
    }

    gradient.setColorAt(0, QColor(0, 0, 0, 0));
    gradient.setColorAt(1, color);

    p.fillRect(rect, gradient);
}



void ShadowOverlayWindow::drawCornerGradient(QPainter& p, const QRect& rect, const QPointF& center, const QColor& color)
{
    QRadialGradient gradient(center, rect.width()); // rect.width() == rect.height() == corner radius
    gradient.setColorAt(0, color);
    gradient.setColorAt(1, QColor(0, 0, 0, 0));

    p.fillRect(rect, gradient);
}



/* ----- SETTERS ----- */
void ShadowOverlayWindow::setOffsetX(const int v)
{
    if (offsetX_ != v) {
        offsetX_ = v;
        syncWithTarget(); // because it influences the position
    }
}



void ShadowOverlayWindow::setOffsetY(const int v)
{
    if (offsetY_ != v) {
        offsetY_ = v;
        syncWithTarget(); // because it influences the position
    }
}



void ShadowOverlayWindow::setMargin(const int v)
{
    if (margin_ != v) {
        margin_ = v;
        syncWithTarget(); // because it influences the position
        update();         // need to repaint
    }
}



void ShadowOverlayWindow::setSoftness(const qreal v)
{
    if (!qFuzzyCompare(softness_, v))
    {
        softness_ = v;
        update();    // only need to repaint
    }
}