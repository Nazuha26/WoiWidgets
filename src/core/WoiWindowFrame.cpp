//********************************************************************
//                                                                   *
//         Project : WoiWidgets                                      *
//         File    : WoiWindowFrame.cpp                              *
//         Author  : Nazuha26                                        *
//                                                                   *
//         Copyright (c) 2025 by Nazuha26                            *
//         All bugs are my own. Handle with care.                    *
//         Copyright last updated on 25.07.2025 at (15:09:56)        *
//                                                                   *
//********************************************************************

//
// Created by Nazar on 16.07.2025.
//

#include "WoiWindowFrame.h"
#include "Icons.h"
#include "ResizePreview.h"
#include "ShadowOverlayWindow.h"
#include "../widgets/WoiTitleBar.h"
#include "../widgets/WoiTitleBarButton.h"
#include "style/WoiThemeEngine.h"

#include <QApplication>
#include <QGraphicsOpacityEffect>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>



WoiWindowFrame::WoiWindowFrame(QWidget* parent, QWidget* child) : WoiThemeWidget(parent),
                                                                  resizePreview_(new ResizePreview(this)),
                                                                  fgOutlineWidth_(4),
                                                                  borderSize_(8),
                                                                  cornerCut_(12),
                                                                  outlineShape_(OutlineShape::SHAPE_1),

                                                                  /* local */
                                                                  isResizing_(false),
                                                                  isDragging_(false),
                                                                  isCollapsed_(false),
                                                                  isResizable_(true),
                                                                  isMaximizable_(true),
                                                                  isMinimizable_(true),
                                                                  detectedDirection_(NONE),
                                                                  widthBeforeCollapse_(-1),
                                                                  heightBeforeCollapse_(-1),
                                                                  wasMaximizedBeforeMinimize_(false)
{
    setWindowFlags(Qt::Window | Qt::FramelessWindowHint);
    setAttribute(Qt::WA_TranslucentBackground);
    setAttribute(Qt::WA_Hover);

    /* --- SHADOW only for WINDOWS os yet --- */
    #if defined(Q_OS_WIN)
        shadowOverlayWindow_ = new ShadowOverlayWindow();
        shadowOverlayWindow_->setTargetWindow(this);
    #else
        shadowOverlayWindow_ = nullptr;
    #endif

    syncStyle();   // synchronize window style with the theme

    mainContainer_ = new QVBoxLayout();
    mainContainer_->setSpacing(0);
    mainContainer_->setContentsMargins(borderSize_, borderSize_, borderSize_, borderSize_);

    titleBar_ = new WoiTitleBar();
    setWindowTitle(titleBar_->woiWindowTitle());

    //connect(titleBar_->btnCollapse(), &WoiTitleBarButton::clicked, this, &WindowFrame::onCollapse);
    connect(titleBar_->btnMinimize(), &WoiTitleBarButton::clicked, this, &WoiWindowFrame::onMinimize);
    connect(titleBar_->btnMaximize(), &WoiTitleBarButton::clicked, this, &WoiWindowFrame::onMaximize);
    connect(titleBar_->btnClose(), &WoiTitleBarButton::clicked, this, &WoiWindowFrame::onClose);

    connect(titleBar_, &WoiTitleBar::dragStarted, this, [this](const QPoint& gp) { onDragStarted(gp); });
    connect(titleBar_, &WoiTitleBar::dragging, this, [this](const QPoint& gp) { onDragMove(gp); });
    connect(titleBar_, &WoiTitleBar::dragFinished, this, [this] { onDragStop(); });
    connect(titleBar_, &WoiTitleBar::doubleClicked, this, [this] { doubleClicked(); });

    mainContainer_->addWidget(titleBar_);


    contentWidget_ = child == nullptr ? new QWidget(this) : child;
    contentWidget_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);
    mainContainer_->addWidget(contentWidget_);

    setLayout(mainContainer_);
}



WoiWindowFrame::~WoiWindowFrame()
{ delete shadowOverlayWindow_; }



/* ========== EVENTS ========== */

void WoiWindowFrame::paintEvent(QPaintEvent* event)
{
    Q_UNUSED(event);
    QPainter p(this);
    //p.setRenderHint(QPainter::Antialiasing);      // off
    p.setPen(Qt::NoPen);



    /* ----- VARIABLES ----- */
    const int w = width();
    const int h = height();


    /* =============== BUILD BG PATHS =============== */
    const QPainterPath bgPath = buildOutlineShapePath(outlineShape_);
    /* -------------------------- */

    /* === PAINT BG PATHS === */
    const bool maximized = isMaximized();
    if (!isCollapsed_ && !maximized)
        p.setBrush(outlineBrush_);
    p.drawPath(bgPath);



    /* =============== BUILD FG PATHS =============== */

    /* --- INNER AREA --- */
    const int IACC_ = cornerCut_ - static_cast<int>(borderSize_ / std::sqrt(3.0));    // inner area corner cut

    /* --- FG --- */
    const int bs    = maximized ? 0 : borderSize_; // border size
    const int iacc_ = maximized ? 0 : IACC_;        // inner area corner cut

    const int top    = bs;
    const int bottom = h - bs;
    const int left   = bs;
    const int right  = w - bs;

    QPainterPath iaMainPath; // inner area main path
    iaMainPath.moveTo(left + iacc_, top);
    iaMainPath.lineTo(right - iacc_, top);
    iaMainPath.lineTo(right, top + iacc_);
    iaMainPath.lineTo(right, bottom - iacc_);
    iaMainPath.lineTo(right - iacc_, bottom);
    iaMainPath.lineTo(left + iacc_, bottom);
    iaMainPath.lineTo(left, bottom - iacc_);
    iaMainPath.lineTo(left, top + iacc_);
    iaMainPath.closeSubpath();
    /* -------------------- */

    // fg outline
    QPainterPathStroker iaMainStroker; // inner area main stroker
    iaMainStroker.setWidth(fgOutlineWidth_);
    QPainterPath iaMainStrokerPath = iaMainStroker.createStroke(iaMainPath); // inner area main stroker path
    iaMainStrokerPath              = iaMainStrokerPath.intersected(iaMainPath);
    // ----------

    /* === PAINT INNER AREA PATHS === */
    p.setBrush(fgColor_);
    p.drawPath(iaMainPath);


    p.setBrush(fgOutlineColor_);
    p.drawPath(iaMainStrokerPath);

    /* ------------------------------------- */

    constexpr int winBtnsW = WoiTitleBar::WIN_BTN_WIDTH;
    constexpr int winBtnsH = WoiTitleBar::WIN_BTN_HEIGHT;
    constexpr int winBtnsM = WoiTitleBar::WIN_BTN_MARGIN;
    constexpr int winBtnsS = WoiTitleBar::WIN_BTN_SPACING;

    const int     btnsCount       = titleBar_->visibleWinButtonsCount();
    const int     totalBtnsWidth  = winBtnsW * btnsCount + winBtnsS * (btnsCount - 1) + winBtnsM * 2;
    constexpr int totalBtnsHeight = winBtnsH + winBtnsM * 2;

    QPainterPath titleBarPath;
    titleBarPath.moveTo(right - totalBtnsWidth, top);
    titleBarPath.lineTo(right - iacc_, top);
    titleBarPath.lineTo(right, top + iacc_);
    titleBarPath.lineTo(right, top + totalBtnsHeight);
    titleBarPath.lineTo(right - (totalBtnsWidth - IACC_), top + totalBtnsHeight);
    titleBarPath.lineTo(right - totalBtnsWidth, top + totalBtnsHeight - IACC_);
    titleBarPath.closeSubpath();

    /* === PAINT PATH === */
    p.setBrush(fgOutlineColor_);
    p.drawPath(titleBarPath);
}



void WoiWindowFrame::mousePressEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton)
        return;
    if (!isResizable_)
        { WOI_DEBUG("Can't resize, reason: isResizable_ = false"); return; }
    if (isMaximized())
        return;
    if (hitTest(event->position().toPoint()) == NONE)
        return;

    isResizing_ = true;

    dragStartGlobalPos_ = event->globalPosition().toPoint();
    dragStartGeometry_  = geometry();

    resizePreview_->showPreview(dragStartGeometry_);
}



void WoiWindowFrame::mouseMoveEvent(QMouseEvent* event)
{
    if (!isResizing_)
        return;

    const QPoint delta     = event->globalPosition().toPoint() - dragStartGlobalPos_;
    const QRect  startRect = dragStartGeometry_;

    const int minW = minimumWidth();
    const int minH = minimumHeight();

    int left   = startRect.left();
    int right  = startRect.right();
    int top    = startRect.top();
    int bottom = startRect.bottom();

    switch (detectedDirection_)
    {
        case LEFT:
        case TOPLEFT:
        case BOTTOMLEFT: {
            left = std::min(startRect.right() - minW, startRect.left() + delta.x());
            break;
        }

        case RIGHT:
        case TOPRIGHT:
        case BOTTOMRIGHT: {
            right = std::max(startRect.left() + minW, startRect.right() + delta.x());
            break;
        }

        default: break;
    }

    switch (detectedDirection_)
    {
        case TOP:
        case TOPLEFT:
        case TOPRIGHT: {
            top = std::min(startRect.bottom() - minH, startRect.top() + delta.y());
            break;
        }

        case BOTTOM:
        case BOTTOMLEFT:
        case BOTTOMRIGHT: {
            bottom = std::max(startRect.top() + minH, startRect.bottom() + delta.y());
            break;
        }

        default: break;
    }

    const QRect newRect(QPoint(left, top), QPoint(right, bottom));
    resizePreview_->updatePreview(newRect);
}



void WoiWindowFrame::mouseReleaseEvent(QMouseEvent* event)
{
    if (!isResizing_)
        return;

    isResizing_           = false;
    const QRect finalRect = resizePreview_->geometry();

    resizePreview_->hidePreview();

    setGeometry(finalRect);
}



void WoiWindowFrame::showEvent(QShowEvent* event)
{
    WoiThemeWidget::showEvent(event);
    centerWindow();
}



void WoiWindowFrame::changeEvent(QEvent* event)
{
    if (event->type() == QEvent::WindowStateChange)
    {
        const Qt::WindowStates st = windowState(); // current state

        if (st == Qt::WindowNoState && wasMaximizedBeforeMinimize_)
        {
            wasMaximizedBeforeMinimize_ = false;
            showMaximized();
            return;
        }

        /* --- Ui update depending on window state ---*/
        if (st == Qt::WindowMaximized)
        {
            mainContainer_->setContentsMargins(0, 0, 0, 0);
            titleBar_->btnMaximize()->setSvgIcon(Icons::path(Icons::Restore));
        }
        else if (st == Qt::WindowNoState)
        {
            mainContainer_->setContentsMargins(borderSize_, borderSize_, borderSize_, borderSize_);
            titleBar_->btnMaximize()->setSvgIcon(Icons::path(Icons::Maximize));
        }
    }
    QWidget::changeEvent(event);
}



bool WoiWindowFrame::event(QEvent* event)
{
    switch (event->type())
    {
        case QEvent::HoverEnter:
        case QEvent::HoverMove: {
            if (!isResizable_)
                { WOI_DEBUG("Can't change resize cursor, reason: isResizable_ = false"); return true; }
            if (isMaximized() || isResizing_)
                return true;
            auto* he           = dynamic_cast<QHoverEvent*>(event);
            detectedDirection_ = hitTest(he->position().toPoint());
            switch (detectedDirection_)
            {
                case TOP:
                case BOTTOM: setCursor(Qt::SizeVerCursor);
                    break;
                case LEFT:
                case RIGHT: setCursor(Qt::SizeHorCursor);
                    break;
                case TOPLEFT:
                case BOTTOMRIGHT: setCursor(Qt::SizeFDiagCursor);
                    break;
                case TOPRIGHT:
                case BOTTOMLEFT: setCursor(Qt::SizeBDiagCursor);
                    break;
                default: setCursor(Qt::ArrowCursor);
                    break;
            }
            return true;
        }
        case QEvent::HoverLeave: {
            setCursor(Qt::ArrowCursor);
            detectedDirection_ = NONE;
            return true;
        }
        default: break;
    }
    return QWidget::event(event);
}



/* ============================ */

/* ===== DRAGGING ===== */
void WoiWindowFrame::onDragStarted(const QPoint& globalPos)
{
    if (isMaximized())
        return;

    isDragging_ = true;
    dragPos_     = globalPos - this->frameGeometry().topLeft();
}



void WoiWindowFrame::onDragMove(const QPoint& globalPos)
{
    if (!isDragging_)
        return;
    this->move(globalPos - dragPos_);
}



void WoiWindowFrame::onDragStop() { isDragging_ = false; }
/* ==================== */

/// Maximize by double-click on title bar
void WoiWindowFrame::doubleClicked() { toggleMaximize(); }

void WoiWindowFrame::onCollapse() { toggleCollapse(); }

void WoiWindowFrame::onMinimize() { showFrameMinimized(); }

void WoiWindowFrame::onMaximize() { toggleMaximize(); }

void WoiWindowFrame::onClose() { close(); }



void WoiWindowFrame::toggleCollapse()
{
    if (isMaximized())
    {
        WOI_DEBUG("Cannot collapse content while window is maximized.");
        return;
    }
    if (isCollapsed_)
    {
        contentWidget_->setVisible(true);
        resize(widthBeforeCollapse_, heightBeforeCollapse_);
        titleBar_->btnCollapse()->setSvgIcon(Icons::path(Icons::Expand));
        isCollapsed_ = false;
    }
    else
    {
        widthBeforeCollapse_  = width();
        heightBeforeCollapse_ = height();

        contentWidget_->setVisible(false);
        adjustSize();
        resize(widthBeforeCollapse_, height());
        titleBar_->btnCollapse()->setSvgIcon(Icons::path(Icons::Collapse));
        isCollapsed_ = true;
    }
    update();
}



void WoiWindowFrame::toggleMaximize()
{
    if (isMaximized())
        showFrameRestored();
    else
        showFrameMaximized();
}



void WoiWindowFrame::showFrameMaximized()
{
    if (!isMaximizable_)
        { WOI_DEBUG("Can't maximize, reason: isMaximizable_ = false"); return; }
    if (windowState() == Qt::WindowMaximized)
        return;

    geometryBeforeMaximize_ = geometry();
    showMaximized();
}



void WoiWindowFrame::showFrameRestored()
{
    if (windowState() == Qt::WindowNoState)
        return;

    showNormal();
    setGeometry(geometryBeforeMaximize_);
}



void WoiWindowFrame::showFrameMinimized()
{
    if (!isMinimizable_)
        { WOI_DEBUG("Can't minimize, reason: isMinimizable_ = false"); return; }
    if (windowState() == Qt::WindowMinimized)
        return;

    wasMaximizedBeforeMinimize_ = isMaximized();
    showMinimized();
}



void WoiWindowFrame::setWoiWindowTitle(const QString& text)
{
    titleBar_->setWoiWindowTitle(text);
    setWindowTitle(text);
}



void WoiWindowFrame::setResizable(const bool v)
{
    isResizable_ = v;
    setMaximizable(v);
    update();
}
void WoiWindowFrame::setMaximizable(const bool v)
{
    isMaximizable_ = v;
    if (isMaximizable_)
        titleBar_->showBtnMaximize();
    else
    {
        titleBar_->hideBtnMaximize();
        if (windowState() == Qt::WindowMaximized)
            showFrameRestored();
    }
    update();
}
void WoiWindowFrame::setMinimizable(const bool v)
{
    isMinimizable_ = v;
    if (isMinimizable_)
        titleBar_->showBtnMinimize();
    else
        titleBar_->hideBtnMinimize();
    update();
}



void WoiWindowFrame::centerWindow()
{
    QRect targetRect;
    const QWidget* parent_ = qobject_cast<QWidget*>(parent());
    if (parent_ && parent_->isVisible())
        targetRect = parent_->geometry();
    else
        targetRect = QGuiApplication::primaryScreen()->availableGeometry();
    const int centerX = targetRect.x() + (targetRect.width() - width()) / 2;
    const int centerY = targetRect.y() + (targetRect.height() - height()) / 2;
    move(centerX, centerY);
}


/* ----- RESIZE PREVIEW WRAPPERS ----- */
int WoiWindowFrame::resizePreviewLineWidth() const
{
    if (resizePreview_)
        return resizePreview_->lineWidth();
    return -1;
}
int WoiWindowFrame::resizePreviewRadius() const
{
    if (resizePreview_)
        return resizePreview_->radius();
    return -1;
}
QColor WoiWindowFrame::resizePreviewColor() const
{
    if (resizePreview_)
        return resizePreview_->color();
    return QColor(0,0,0,0);
}
LineStyle WoiWindowFrame::resizePreviewLineStyle() const
{
    if (resizePreview_)
        return resizePreview_->lineStyle();
    return LineStyle::SOLID_LINE;
}



void WoiWindowFrame::setResizePreviewLineWidth(const int v)
{
    if (resizePreview_)
        resizePreview_->setLineWidth(v);
}
void WoiWindowFrame::setResizePreviewRadius(const int v)
{
    if (resizePreview_)
        resizePreview_->setRadius(v);
}
void WoiWindowFrame::setResizePreviewLineStyle(const LineStyle style)
{
    if (resizePreview_)
        resizePreview_->setLineStyle(style);
}



/* ----- SHADOW WINDOW WRAPPERS ----- */
int WoiWindowFrame::shadowOffsetX() const
{
    if (shadowOverlayWindow_)
        return shadowOverlayWindow_->offsetX();
    return -1;
}
int WoiWindowFrame::shadowOffsetY() const
{
    if (shadowOverlayWindow_)
        return shadowOverlayWindow_->offsetY();
    return -1;
}
int WoiWindowFrame::shadowMargin() const
{
    if (shadowOverlayWindow_)
        return shadowOverlayWindow_->margin();
    return -1;
}
qreal WoiWindowFrame::shadowSoftness() const
{
    if (shadowOverlayWindow_)
        return shadowOverlayWindow_->softness();
    return -1;
}
QColor WoiWindowFrame::shadowColor() const
{
    if (shadowOverlayWindow_)
        return shadowOverlayWindow_->color();
    return QColor(0,0,0,0);
}
qreal WoiWindowFrame::shadowTopAlpha() const
{
    if (shadowOverlayWindow_)
        return shadowOverlayWindow_->topAlpha();
    return -1;
}
qreal WoiWindowFrame::shadowBottomAlpha() const
{
    if (shadowOverlayWindow_)
        return shadowOverlayWindow_->bottomAlpha();
    return -1;
}

void WoiWindowFrame::setShadowOffsets(const int offsetX, const int offsetY)
{
    if (shadowOverlayWindow_) {
        shadowOverlayWindow_->setOffsetX(offsetX);
        shadowOverlayWindow_->setOffsetY(offsetY);
    }
}
void WoiWindowFrame::setShadowMargin(const int v)
{
    if (shadowOverlayWindow_)
        shadowOverlayWindow_->setMargin(v);
}
void WoiWindowFrame::setShadowSoftness(const qreal v)
{
    if (shadowOverlayWindow_)
        shadowOverlayWindow_->setSoftness(v);
}



/* ===== CORE ===== */
void WoiWindowFrame::installModalFocusInterceptor()
{
    if (QWidget* parent_ = qobject_cast<QWidget*>(parent()))
        parent_->installEventFilter(this);
}

WoiWindowFrame::ResizeDirection WoiWindowFrame::hitTest(const QPoint& pos) const
{
    const int x = pos.x();
    const int y = pos.y();

    const bool topLeft     = x < cornerCut_ && y < cornerCut_;
    const bool topRight    = x > width() - cornerCut_ && y < cornerCut_;
    const bool bottomLeft  = x < cornerCut_ && y > height() - cornerCut_;
    const bool bottomRight = x > width() - cornerCut_ && y > height() - cornerCut_;

    const bool left   = x < borderSize_;
    const bool right  = x > width() - borderSize_;
    const bool top    = y < borderSize_;
    const bool bottom = y > height() - borderSize_;

    if (topLeft)
        return TOPLEFT;
    if (topRight)
        return TOPRIGHT;
    if (bottomLeft)
        return BOTTOMLEFT;
    if (bottomRight)
        return BOTTOMRIGHT;
    if (left)
        return LEFT;
    if (right)
        return RIGHT;
    if (top)
        return TOP;
    if (bottom)
        return BOTTOM;

    return NONE;
}



QPainterPath WoiWindowFrame::buildOutlineShapePath(const OutlineShape shape) const
{
    const int w = width();
    const int h = height();

    const int smallCornerCut_ = borderSize_ / 2;

    QPainterPath path;

    switch (shape)
    {
        case OutlineShape::SHAPE_1: {
            const int lTt = w / 4; // left Top top
            const int lTl = h / 4; // left Top left

            const int rTt = w / 4; // right Top top
            const int rTr = h / 4; // right Top right

            const int lBb = w / 4; // left Bottom bottom
            const int lBl = h / 4; // left Bottom left

            const int rBb = w / 4; // right Bottom bottom
            const int rBr = h / 4; // right Bottom right

            /* --- BG --- */
            path.moveTo(cornerCut_, 0);
            path.lineTo(lTt, 0);
            path.lineTo(lTt + smallCornerCut_, smallCornerCut_);
            path.lineTo(w - (rTt + smallCornerCut_), smallCornerCut_);
            path.lineTo(w - rTt, 0);
            path.lineTo(w - cornerCut_, 0);
            path.lineTo(w, cornerCut_);
            path.lineTo(w, rTr);
            path.lineTo(w - smallCornerCut_, rTr + smallCornerCut_);
            path.lineTo(w - smallCornerCut_, h - (rBr + smallCornerCut_));
            path.lineTo(w, h - rBr);
            path.lineTo(w, h - cornerCut_);
            path.lineTo(w - cornerCut_, h);
            path.lineTo(w - rBb, h);
            path.lineTo(w - (rBb + smallCornerCut_), h - smallCornerCut_);
            path.lineTo(lBb + smallCornerCut_, h - smallCornerCut_);
            path.lineTo(lBb, h);
            path.lineTo(cornerCut_, h);
            path.lineTo(0, h - cornerCut_);
            path.lineTo(0, h - lBl);
            path.lineTo(smallCornerCut_, h - (lBl + smallCornerCut_));
            path.lineTo(smallCornerCut_, lTl + smallCornerCut_);
            path.lineTo(0, lTl);
            path.lineTo(0, cornerCut_);
            path.closeSubpath();
            break;
        }

        case OutlineShape::SHAPE_2: {
            constexpr int top    = 0;
            const int     bottom = h;
            constexpr int left   = 0;
            const int     right  = w;

            path.moveTo(left + cornerCut_, top);
            path.lineTo(right - cornerCut_, top);
            path.lineTo(right, top + cornerCut_);
            path.lineTo(right, bottom - cornerCut_);
            path.lineTo(right - cornerCut_, bottom);
            path.lineTo(left + cornerCut_, bottom);
            path.lineTo(left, bottom - cornerCut_);
            path.lineTo(left, top + cornerCut_);
            path.closeSubpath();
            break;
        }
        case OutlineShape::SHAPE_3: {
            path.addRoundedRect(rect(), cornerCut_, cornerCut_);
            break;
        }
        default: path.addRect(rect());
            break;
    }
    return path;
}



/* ----- THEME MANAGMENT ----- */
void WoiWindowFrame::applyTheme(const WoiTheme& theme)
{ applyStyle(theme.windowFrame); }



void WoiWindowFrame::applyStyle(const WoiTheme::WindowFrameStyle& style)
{
    /* --- colors --- */
    fgColor_ = style.fgColor;
    fgOutlineColor_ = style.fgOutlineColor;
    outlineBrush_ = style.outlineBrush;
    update();
}



void WoiWindowFrame::syncStyle()
{ applyStyle(ThemeEngine->currentTheme().windowFrame); }
/* ----- THEME MANAGMENT ----- */