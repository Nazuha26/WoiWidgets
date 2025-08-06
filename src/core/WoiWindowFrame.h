//********************************************************************
//                                                                   *
//         Project : WoiWidgets                                      *
//         File    : WoiWindowFrame.h                                *
//         Author  : Nazuha26                                        *
//                                                                   *
//         Copyright (c) 2025 by Nazuha26                            *
//         All bugs are my own. Handle with care.                    *
//         Copyright last updated on 25.07.2025 at (15:35:30)        *
//                                                                   *
//********************************************************************

//
// Created by Nazar on 16.07.2025.
//

#ifndef WINDOWFRAME_H
#define WINDOWFRAME_H

#include <QHBoxLayout>
#include <QLabel>
#include <QMouseEvent>
#include <QWidget>

#include "WoiMacros.h"
#include "style/WoiThemeWidget.h"

#if defined(Q_OS_WINDOWS)
#include <windows.h>
#endif

class ShadowOverlayWindow;
class ResizePreview;
class WoiTitleBar;
enum class LineStyle;

enum class OutlineShape {
    SHAPE_1,
    SHAPE_2,
    SHAPE_3
};

class WoiWindowFrame final : public WoiThemeWidget {
    Q_OBJECT

public:
    /* --- GETTERS --- */
    /* - style colors -*/
    [[nodiscard]] const QColor& fgColor() const { return fgColor_; }
    [[nodiscard]] const QColor& fgOutlineColor() const { return fgOutlineColor_; }
    [[nodiscard]] const QBrush& outlineBrush() const { return outlineBrush_; }

    /* - style geometry -*/
    [[nodiscard]] int fgOutlineWidth() const { return fgOutlineWidth_; }
    [[nodiscard]] int borderSize() const { return borderSize_; }
    [[nodiscard]] int cornerCut() const { return cornerCut_; }
    [[nodiscard]] OutlineShape outlineShape() const { return outlineShape_; }

    /* --- resize preview --- */
    [[nodiscard]] int resizePreviewLineWidth() const;
    [[nodiscard]] int resizePreviewRadius() const;
    [[nodiscard]] QColor resizePreviewColor() const;
    [[nodiscard]] LineStyle resizePreviewLineStyle() const;

    /* --- shadow window --- */
    [[nodiscard]] int shadowOffsetX() const;
    [[nodiscard]] int shadowOffsetY() const;
    [[nodiscard]] int shadowMargin() const;
    [[nodiscard]] qreal shadowSoftness() const;
    [[nodiscard]] QColor shadowColor() const;
    [[nodiscard]] qreal shadowTopAlpha() const;
    [[nodiscard]] qreal shadowBottomAlpha() const;

    /* --- SETTERS --- */
    /* - style geometry - */
    void setFgOutlineWidth(const int v) { STYLE_SETTER(fgOutlineWidth_, v); }
    void setBorderSize(const int v) { STYLE_SETTER(borderSize_, v); }
    void setCornerCut(const int v) { STYLE_SETTER(cornerCut_, v); }
    void setOutlineShape(const OutlineShape& shape) { STYLE_SETTER(outlineShape_, shape); }

    /* --- resize preview --- */
    void setResizePreviewLineWidth(int v);
    void setResizePreviewRadius(int v);
    void setResizePreviewLineStyle(LineStyle style);

    /* --- shadow window --- */
    void setShadowOffsets(int offsetX, int offsetY);
    void setShadowMargin(int v);
    void setShadowSoftness(qreal v);


    /* --- PUBLIC CORE --- */
    void toggleCollapse();  /// toggle expand/collapse content
    void toggleMaximize();
    void showFrameMaximized();
    void showFrameRestored();
    void showFrameMinimized();
    void setWoiWindowTitle(const QString& text);
    void setResizable(bool v);
    void setMaximizable(bool v);
    void setMinimizable(bool v);
    void centerWindow();    // center the window frame

    explicit WoiWindowFrame(QWidget* parent = nullptr, QWidget* child = nullptr);
    ~WoiWindowFrame();

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void showEvent(QShowEvent* event) override;
    void changeEvent(QEvent*) override;
    bool event(QEvent* event) override;

    void applyTheme(const WoiTheme& theme) override;

private:
    enum ResizeDirection {
        NONE,
        LEFT,
        RIGHT,
        TOP,
        BOTTOM,
        TOPLEFT,
        TOPRIGHT,
        BOTTOMLEFT,
        BOTTOMRIGHT
    };

    /* ----- UI ----- */
    ResizePreview* resizePreview_;
    ShadowOverlayWindow* shadowOverlayWindow_;
    QVBoxLayout* mainContainer_;
    WoiTitleBar* titleBar_;
    QWidget* contentWidget_;

    /* ----- PARAMS ----- */
    bool isResizing_; // only getter
    bool isDragging_; // only getter
    bool isCollapsed_; // only getter
    bool isResizable_;      // false = can't resize and maximize
    bool isMaximizable_;    // false = can't maximize
    bool isMinimizable_;    // false = can't minimize

    /* --- FIELDS --- */
    QPoint          dragPos_;            // for window drag
    QPoint          dragStartGlobalPos_; // for resize preview
    QRect           dragStartGeometry_;  // for resize preview
    ResizeDirection detectedDirection_;

    int   widthBeforeCollapse_;
    int   heightBeforeCollapse_;
    QRect geometryBeforeMaximize_;
    bool  wasMaximizedBeforeMinimize_;


    /* ---------- STYLE ---------- */

    /* --- COLORS --- */
    QColor fgColor_;
    QColor fgOutlineColor_;
    QBrush outlineBrush_;

    /* --- GEOMETRY --- */
    int fgOutlineWidth_;
    int borderSize_;
    int cornerCut_;
    OutlineShape outlineShape_;



    /* --- PARAMS --- */
    // nothing

    /* --- CORE --- */
    void installModalFocusInterceptor();
    [[nodiscard]] ResizeDirection hitTest(const QPoint& pos) const;
    [[nodiscard]] QPainterPath    buildOutlineShapePath(OutlineShape shape) const;
    void                          applyStyle(const WoiTheme::WindowFrameStyle& style);
    void                          syncStyle();

private slots:
    void onDragStarted(const QPoint& globalPos);
    void onDragMove(const QPoint& globalPos);
    void onDragStop();
    void doubleClicked();

    void onCollapse();
    void onMinimize();
    void onMaximize();
    void onClose();
};



#endif //WINDOWFRAME_H