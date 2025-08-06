//********************************************************************
//                                                                   *
//         Project : WoiWidgets                                      *
//         File    : WoiTitleBarButton.cpp                           *
//         Author  : Nazuha26                                        *
//                                                                   *
//         Copyright (c) 2025 by Nazuha26                            *
//         All bugs are my own. Handle with care.                    *
//         Copyright last updated on 25.07.2025 at (09:23:45)        *
//                                                                   *
//********************************************************************

//
// Created by Nazar on 23.07.2025.
//

#include "WoiTitleBarButton.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>



WoiTitleBarButton::WoiTitleBarButton(const QString& svgPath, QWidget* parent) : QWidget(parent),
    normalColor_(QColor(0, 0, 255)),
    hoverColor_(QColor(0, 255, 0)),
    pressColor_(QColor(255, 0, 0)),
    contentScaleFactor_(0.7f),
    svgPath_(svgPath)
{
    setMouseTracking(true);

    svgRenderer_ = new QSvgRenderer(svgPath_, this);

    updateCurrentColor();
}



WoiTitleBarButton::WoiTitleBarButton(const QString& svgPath, QColor normalColor, QColor hoverColor, QColor pressColor,
                                     float contentScaleFactor, QWidget* parent) : WoiTitleBarButton(svgPath, parent)
{
    setNormalColor(normalColor);
    setHoverColor(hoverColor);
    setPressColor(pressColor);
    setContentScaleFactor(contentScaleFactor);
}



WoiTitleBarButton::~WoiTitleBarButton() = default;



QSize WoiTitleBarButton::sizeHint() const
{ return {24, 24}; }


/* ------------------------ EVENTS ------------------------ */

void WoiTitleBarButton::paintEvent(QPaintEvent* event)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(Qt::NoPen);
    p.setBrush(currentColor_);
    p.drawEllipse(rect()); // shape

    /* --- SVG RENDER --- */
    const int w = width();
    const int h = height();

    const bool hasIcon  = svgRenderer_ && svgRenderer_->isValid();
    const float csf = contentScaleFactor_ > 0.0f && contentScaleFactor_ < 1.0f ? contentScaleFactor_ : 0.5f /* 0.5 – fallback */; // valid contentScaleFactor
    if (hasIcon)
    {
        const float rawIconSize = qMin(w, h) * csf;
        const int   iconSize = qRound(rawIconSize);
        const QRect iconRect((w - iconSize) / 2, (h - iconSize) / 2, iconSize, iconSize);
        svgRenderer_->render(&p, iconRect);
    }
}



void WoiTitleBarButton::mousePressEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton) return;

    if (currentState_ != PRESS)
    {
        currentState_ = PRESS;
        currentColor_ = pressColor_;
        update();
    }
}



void WoiTitleBarButton::mouseReleaseEvent(QMouseEvent* event)
{
    if (rect().contains(event->pos()))
    {
        currentState_ = HOVER;
        currentColor_ = hoverColor_;
        emit clicked();
    }
    else
    {
        currentState_ = NORMAL;
        currentColor_ = normalColor_;
    }

    update();
}



void WoiTitleBarButton::enterEvent(QEnterEvent* event)
{
    Q_UNUSED(event)
    if (currentState_ != HOVER)
    {
        currentState_ = HOVER;
        currentColor_ = hoverColor_;
        update();
    }
}



void WoiTitleBarButton::leaveEvent(QEvent* event)
{
    Q_UNUSED(event)
    if (currentState_ != NORMAL)
    {
        currentState_ = NORMAL;
        currentColor_ = normalColor_;
        update();
    }
}

/* -------------------------------------------------------- */



void WoiTitleBarButton::setSvgIcon(const QString& path)
{
    if (svgPath_ == path) return;

    svgPath_ = path;
    if (!svgRenderer_) svgRenderer_ = new QSvgRenderer(this);

    svgRenderer_->load(path);
    update();
}



void WoiTitleBarButton::setNormalColor(const QColor& color)
{
    if (normalColor_ == color) return;
    normalColor_ = color;
    updateCurrentColor();
}



void WoiTitleBarButton::setHoverColor(const QColor& color)
{
    if (hoverColor_ == color) return;
    hoverColor_ = color;
    updateCurrentColor();
}



void WoiTitleBarButton::setPressColor(const QColor& color)
{
    if (pressColor_ == color) return;
    pressColor_ = color;
    updateCurrentColor();
}



/* --------------- CORE --------------- */
void WoiTitleBarButton::updateCurrentColor()
{
    switch (currentState_)
    {
        case NORMAL: currentColor_ = normalColor_;
            break;
        case HOVER: currentColor_ = hoverColor_;
            break;
        case PRESS: currentColor_ = pressColor_;
            break;
        default: currentColor_ = QColor(255, 0, 0);
            break; // fallback
    }
    update();
}