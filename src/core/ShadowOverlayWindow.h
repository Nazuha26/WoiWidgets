//********************************************************************
//                                                                   *
//         Project : WoiWidgets                                      *
//         File    : ShadowOverlayWindow.h                           *
//         Author  : Nazuha26                                        *
//                                                                   *
//         Copyright (c) 2025 by Nazuha26                            *
//         All bugs are my own. Handle with care.                    *
//         Copyright last updated on 03.08.2025 at (19:34:12)        *
//                                                                   *
//********************************************************************

//         Created by Nazuha26 in CLion
//         Date: 03.08.2025
//         Time: 19:34:11

#ifndef SHADOWOVERLAYWINDOW_H
#define SHADOWOVERLAYWINDOW_H



#include <QPainterPath>

#include "style/WoiThemeWidget.h"



class ShadowOverlayWindow final : public WoiThemeWidget {
    Q_OBJECT

public:

    /* --- GETTERS --- */
    [[nodiscard]] int offsetX() const { return offsetX_; };
    [[nodiscard]] int offsetY() const { return offsetY_; }
    [[nodiscard]] int margin() const { return margin_; }
    [[nodiscard]] qreal softness() const { return softness_; }
    [[nodiscard]] const QColor& color() const { return color_; }
    [[nodiscard]] qreal topAlpha() const { return topAlpha_; }
    [[nodiscard]] qreal bottomAlpha() const { return bottomAlpha_; }

    /* --- SETTERS --- */
    void setOffsetX(int v);
    void setOffsetY(int v);
    void setMargin(int v);
    void setSoftness(qreal v);

    /* --- PUBLIC CORE --- */
    void setTargetWindow(QWidget* target);  // public core

    explicit ShadowOverlayWindow(QWidget* parent = nullptr);
    ~ShadowOverlayWindow() = default;

protected:
    void paintEvent(QPaintEvent* event) override;
    bool eventFilter(QObject* watched, QEvent* event) override;

    void applyTheme(const WoiTheme& theme) override;

private:
    int    offsetX_;
    int    offsetY_;
    int    margin_;
    qreal  softness_;
    QColor color_;
    qreal  topAlpha_;
    qreal  bottomAlpha_;

    /* --- CORE --- */
    QWidget*    targetWindow_;
    void        syncWithTarget();
    void        syncZOrder() const;
    static void drawSideGradient(QPainter& p, const QRect& rect, Qt::Orientation orientation, const QColor& color,
                                 bool      reverse = false);
    static void drawCornerGradient(QPainter& p, const QRect& rect, const QPointF& center, const QColor& color);

    void applyStyle(const WoiTheme::ShadowOverlayWindowStyle& style);
    void syncStyle();
};



#endif //SHADOWOVERLAYWINDOW_H
