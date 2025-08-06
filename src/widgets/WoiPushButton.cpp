//********************************************************************
//                                                                   *
//         Project : WoiWidgets                                      *
//         File    : WoiPushButton.cpp                               *
//         Author  : Nazuha26                                        *
//                                                                   *
//         Copyright (c) 2025 by Nazuha26                            *
//         All bugs are my own. Handle with care.                    *
//         Copyright last updated on 25.07.2025 at (09:23:45)        *
//                                                                   *
//********************************************************************

//
// Created by Nazar on 14.07.2025.
//

#include "WoiPushButton.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

#include "../core/style/WoiThemeEngine.h"



WoiPushButton::WoiPushButton(const QString& svgIconPath, const QString& text, QWidget* parent) : WoiThemeWidget(parent),
    svgIconPath_(svgIconPath),
    text_(text),
    iconPos_(ContentPosition::LEFT),
    textPos_(ContentPosition::RIGHT),
    iconSize_(24),
    textSize_(14.f),
    iconSpacing_(6),
    contentMargin_(2),
    cornerCut_(6),

    /* local */
    currentState_(NORMAL)
{
    setMouseTracking(true);
    setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);
    setFocusPolicy(Qt::StrongFocus);

    syncStyle();   // synchronize push button style with the theme

    svgRenderer_ = new QSvgRenderer(svgIconPath_, this);

    updateCurrentColor();
}



QSize WoiPushButton::sizeHint() const
{
    const bool hasIcon = svgRenderer_ && svgRenderer_->isValid();
    const bool hasText = !text_.trimmed().isEmpty();

    QFont f = font();
    f.setPointSizeF(textSize_);
    const QFontMetrics fm(f);

    QSize sz = effectiveContentSize(fm, hasIcon, hasText);

    /* add margin */
    sz.rwidth()  += contentMargin_ * 2;
    sz.rheight() += contentMargin_ * 2;

    return sz;
}



QSize WoiPushButton::minimumSizeHint() const
{ return sizeHint(); }



/* ----- THEME MANAGMENT ----- */
void WoiPushButton::applyTheme(const WoiTheme& theme)
{ applyStyle(theme.pushButton); }



void WoiPushButton::applyStyle(const WoiTheme::PushButtonStyle& style)
{
    /* --- colors --- */
    fgColor_ = style.fgColor;
    normalColor_ = style.normalColor;
    hoverColor_ = style.hoverColor;
    pressColor_ = style.pressColor;

    updateCurrentColor();
}



void WoiPushButton::syncStyle()
{ applyStyle(ThemeEngine->currentTheme().pushButton); }
/* ----- THEME MANAGMENT ----- */



/* ------------------------ EVENTS ------------------------ */

void WoiPushButton::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing);
    p.setPen(Qt::NoPen);

    const int w = width();
    const int h = height();

    /* ----- SHAPE ----- */
    QPainterPath path;
    path.moveTo(cornerCut_, 0);
    path.lineTo(w, 0);
    path.lineTo(w, h - cornerCut_);
    path.lineTo(w - cornerCut_, h);
    path.lineTo(0, h);
    path.lineTo(0, cornerCut_);
    path.closeSubpath();
    p.setClipPath(path);

    p.setBrush(currentColor_); // draw shape
    p.drawPath(path);

    /* --- overlay gradient --- */
    QLinearGradient overlayGradient(0, 0, 0, height());
    overlayGradient.setColorAt(0, QColor(0,0,0,0));
    overlayGradient.setColorAt(1, QColor(0xbb, 0xbb, 0xbb, 0xB2)); // alpha = 70%
    p.fillPath(path, overlayGradient);

    /* --- glare --- */
    const int glareWidth = qRound(static_cast<float>(w) / 5.0f);
    QPainterPath glarePath;
    glarePath.moveTo(w - glareWidth, 0);
    glarePath.lineTo(w, 0);
    glarePath.lineTo(w, h);
    glarePath.lineTo(w - (cornerCut_ + glareWidth), h);
    glarePath.lineTo(w - glareWidth, h - cornerCut_);
    glarePath.closeSubpath();

    p.setBrush(QColor(0xff, 0xff, 0xff, 0x33)); // draw glare, alpha = 20%
    p.drawPath(glarePath);
    /* -------------------- */

    /* ----- FOCUS ----- */
    if (hasFocus()) {
        const QPen focusPen(Qt::yellow, 4, Qt::DotLine);
        p.setPen(focusPen);
        p.setBrush(Qt::NoBrush);
        p.drawPath(path);
    }


    /* ----- DATA ----- */
    const bool hasIcon = svgRenderer_ && svgRenderer_->isValid();
    const bool hasText = !text_.trimmed().isEmpty();
    if (!hasIcon && !hasText)
        return; // nothing to draw

    const int availW = w - 2 * contentMargin_;
    const int availH = h - 2 * contentMargin_;

    /* ----- FONT ----- */
    QFont f = font();
    f.setPointSizeF(textSize_);
    p.setFont(f);
    const QFontMetrics fm(f);

    /* =================================================================== */
    /*                         ONLY ICON in center                         */
    /* =================================================================== */
    if (hasIcon && !hasText)
    {
        int x = (w - iconSize_) / 2;
        int y = (h - iconSize_) / 2;

        x = std::clamp(x, contentMargin_, w - contentMargin_ - iconSize_);
        y = std::clamp(y, contentMargin_, h - contentMargin_ - iconSize_);

        svgRenderer_->render(&p, QRect(x, y, iconSize_, iconSize_));
        return;
    }


    /* =================================================================== */
    /*                    ONLY TEXT relative to textPos_                   */
    /* =================================================================== */
    const int textW   = fm.horizontalAdvance(text_);
    const int textH   = fm.height();
    const int centerY = h / 2;

    if (!hasIcon && hasText)
    {
        int x = 0;
        switch (textPos_)
        {
            case ContentPosition::LEFT: x = contentMargin_;
                break;
            case ContentPosition::CENTER: x = (w - textW) / 2;
                break;
            case ContentPosition::RIGHT: x = w - contentMargin_ - textW;
                break;
        }
        const int y = centerY + textH / 2 - fm.descent();

        p.setPen(fgColor_);
        p.drawText(x, y, text_);
        return;
    }

    /* =================================================================== */
    /*   ICON and TEXT:                                                    */
    /*       * icon: LEFT / RIGHT  (iconPos_)                              */
    /*       * text: LEFT / CENTER / RIGHT  (textPos_),                    */
    /* =================================================================== */
    QRect     iconRect;
    int       textX = 0;
    const int textAvailableWidth = availW - (iconSize_ + iconSpacing_);

    /* ----- ICON ----- */
    if (iconPos_ == ContentPosition::LEFT)
        iconRect = QRect(contentMargin_, centerY - iconSize_ / 2, iconSize_, iconSize_);
    else if (iconPos_ == ContentPosition::RIGHT)
        iconRect = QRect(w - contentMargin_ - iconSize_, centerY - iconSize_ / 2, iconSize_, iconSize_);

    /* ----- TEXT ----- */
    switch (textPos_)
    {
        case ContentPosition::LEFT: textX = contentMargin_;
            break;
        case ContentPosition::CENTER: {
            if (iconPos_ == ContentPosition::LEFT)
                textX = iconRect.right() + iconSpacing_ + (textAvailableWidth - textW) / 2;
            else if (iconPos_ == ContentPosition::RIGHT)
                textX = contentMargin_ + (textAvailableWidth - textW) / 2;
            break;
        }
        case ContentPosition::RIGHT: textX = w - contentMargin_ - textW;
            break;
    }

    /* --- offset, if icon and text positions are the same --- */
    if (textPos_ == ContentPosition::LEFT && iconPos_ == ContentPosition::LEFT)
        textX = iconRect.right() + iconSpacing_;

    if (textPos_ == ContentPosition::RIGHT && iconPos_ == ContentPosition::RIGHT)
        textX = iconRect.left() - iconSpacing_ - textW;

    const int textY = centerY + textH / 2 - fm.descent();

    /* ----- DRAW ----- */
    svgRenderer_->render(&p, iconRect);

    p.setPen(fgColor_);
    p.drawText(textX, textY, text_);
}



void WoiPushButton::enterEvent(QEnterEvent* event)
{
    Q_UNUSED(event)
    if (currentState_ != HOVER)
    {
        currentState_ = HOVER;
        currentColor_ = hoverColor_;
        update();
    }
}



void WoiPushButton::leaveEvent(QEvent* event)
{
    Q_UNUSED(event)
    if (currentState_ != NORMAL)
    {
        currentState_ = NORMAL;
        currentColor_ = normalColor_;
        update();
    }
}



void WoiPushButton::mousePressEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton) return;

    if (currentState_ != PRESS)
    {
        currentState_ = PRESS;
        currentColor_ = pressColor_;
        update();
    }
}



void WoiPushButton::mouseReleaseEvent(QMouseEvent* event)
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


/* -------------------------------------------------------- */


/* ---------- SETTERS ---------- */
/*void WoiPushButton::setNormalColor(const QColor& color)
{
    if (normalColor_ == color)
        return;
    normalColor_ = color;
    updateCurrentColor();
}

void WoiPushButton::setHoverColor(const QColor& color)
{
    if (hoverColor_ == color)
        return;
    hoverColor_ = color;
    updateCurrentColor();
}

void WoiPushButton::setPressColor(const QColor& color)
{
    if (pressColor_ == color)
        return;
    pressColor_ = color;
    updateCurrentColor();
}*/

void WoiPushButton::setSvgIcon(const QString& path)
{
    if (svgIconPath_ == path)
        return;

    svgIconPath_ = path;
    if (!svgRenderer_)
        svgRenderer_ = new QSvgRenderer(this);

    svgRenderer_->load(path);
    update();
}

void WoiPushButton::setText(const QString& text)
{
    text_ = text.trimmed();
    invalidateGeometry();
}

void WoiPushButton::setTextSize(const float v)
{
    textSize_ = v;
    invalidateGeometry();
}

void WoiPushButton::setIconSize(const int v)
{
    iconSize_ = v;
    invalidateGeometry();
}

void WoiPushButton::setIconSpacing(const int v)
{
    iconSpacing_ = v;
    invalidateGeometry();
}

void WoiPushButton::setContentMargin(const int v)
{
    contentMargin_ = v;
    invalidateGeometry();
}



/* --------------- CORE --------------- */
void WoiPushButton::updateCurrentColor()
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



QSize WoiPushButton::effectiveContentSize(const QFontMetrics& fm, const bool hasIcon, const bool hasText) const
{
    const int textW = fm.horizontalAdvance(text_);
    const int textH = fm.height();

    if (hasIcon && !hasText)
        return { iconSize_, iconSize_ };

    if (!hasIcon && hasText)
        return { textW, textH };

    if (hasIcon && hasText)
        return { iconSize_ + iconSpacing_ + textW, std::max(iconSize_, textH) };

    return { 1, 1 };        /* fallback, if no icon and no text */
}



void WoiPushButton::invalidateGeometry()
{
    updateGeometry();
    update();
}