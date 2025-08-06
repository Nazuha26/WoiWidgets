//********************************************************************
//                                                                   *
//         Project : WoiWidgets                                      *
//         File    : ResizePreview.cpp                               *
//         Author  : Nazuha26                                        *
//                                                                   *
//         Copyright (c) 2025 by Nazuha26                            *
//         All bugs are my own. Handle with care.                    *
//         Copyright last updated on 25.07.2025 at (09:23:45)        *
//                                                                   *
//********************************************************************

//
// Created by Nazar on 11.07.2025.
//

#include <QPainter>
#include <QPainterPath>

#include "ResizePreview.h"

#include "style/WoiThemeEngine.h"



ResizePreview::ResizePreview(QWidget* parent) : WoiThemeWidget(parent),
                                                lineWidth_(4),
                                                radius_(12),
                                                lineStyle_(LineStyle::DASHED_LINE)
{
    setWindowFlags(Qt::Tool | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TransparentForMouseEvents);
    setAttribute(Qt::WA_TranslucentBackground);

    syncStyle();   // synchronize resize preview style with the theme

    setWindowTitle("resizepreview");
    hidePreview();
}



void ResizePreview::paintEvent(QPaintEvent* event)
{
    if (width() <= 0 || height() <= 0)
        return;

    QPainter painter(this);
    painter.setRenderHint(QPainter::Antialiasing);

    QPen pen(color_, lineWidth_);
    pen.setJoinStyle(Qt::RoundJoin);

    switch (lineStyle_)
    {
        case LineStyle::DASHED_LINE: {
            pen.setStyle(Qt::DashLine);
            pen.setCapStyle(Qt::FlatCap);
            break;
        }
        case LineStyle::DOTTED_LINE: {
            pen.setStyle(Qt::DotLine);
            pen.setCapStyle(Qt::FlatCap);
            break;
        }
        case LineStyle::DASH_DOT_LINE: {
            pen.setStyle(Qt::DashDotLine);
            pen.setCapStyle(Qt::FlatCap);
            break;
        }
        default: {
            pen.setStyle(Qt::SolidLine);
            pen.setCapStyle(Qt::SquareCap);
            break;
        }
    }

    painter.setPen(pen);
    painter.setBrush(Qt::NoBrush);

    QPainterPath path;
    path.addRoundedRect(rect().adjusted(lineWidth_ / 2, lineWidth_ / 2, -lineWidth_ / 2, -lineWidth_ / 2),
                        radius_, radius_);
    painter.drawPath(path);
}



/* ----- THEME MANAGMENT ----- */
void ResizePreview::applyTheme(const WoiTheme& theme)
{ applyStyle(theme.resizePreview); }



void ResizePreview::applyStyle(const WoiTheme::ResizePreviewStyle& style)
{
    color_ = style.color;
    update();
}



void ResizePreview::syncStyle()
{ applyStyle(ThemeEngine->currentTheme().resizePreview); }
/* ----- THEME MANAGMENT ----- */



void ResizePreview::updatePreview(const QRect& rect)
{
    setGeometry(rect);
    update();
}



void ResizePreview::showPreview(const QRect& rect)
{
    setGeometry(rect);
    show();
    raise();


    /* may add animation here */
}



void ResizePreview::hidePreview()
{ hide(); }
