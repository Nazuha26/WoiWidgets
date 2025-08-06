//********************************************************************
//                                                                   *
//         Project : WoiWidgets                                      *
//         File    : ResizePreview.h                                 *
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

#ifndef RESIZEPREVIEW_H
#define RESIZEPREVIEW_H

#include <QWidget>

#include "WoiMacros.h"
#include "style/WoiThemeWidget.h"



enum class LineStyle {
    SOLID_LINE,
    DASHED_LINE,
    DOTTED_LINE,
    DASH_DOT_LINE
};

class ResizePreview final : public WoiThemeWidget {
    Q_OBJECT

public:

    /* --- GETTERS --- */
    [[nodiscard]] int           lineWidth() const { return lineWidth_; }
    [[nodiscard]] int           radius() const { return radius_; }
    [[nodiscard]] const QColor& color() const { return color_; }
    [[nodiscard]] LineStyle     lineStyle() const { return lineStyle_; }

    /* --- SETTERS --- */
    void setLineWidth(const int v) { STYLE_SETTER(lineWidth_, v); }
    void setRadius(const int v) { STYLE_SETTER(radius_, v); }
    void setLineStyle(const LineStyle style) { STYLE_SETTER(lineStyle_, style); }

    /* --- PUBLIC CORE --- */
    void updatePreview(const QRect& rect);
    void showPreview(const QRect& rect);
    void hidePreview();

    explicit ResizePreview(QWidget* parent = nullptr);
    ~ResizePreview() = default;

protected:
    void paintEvent(QPaintEvent* event) override;

    void applyTheme(const WoiTheme& theme) override;

private:

    int       lineWidth_;
    int       radius_;
    QColor    color_;
    LineStyle lineStyle_;

    /* --- CORE --- */
    void applyStyle(const WoiTheme::ResizePreviewStyle& style);
    void syncStyle();
};



#endif //RESIZEPREVIEW_H
