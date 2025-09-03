//********************************************************************
//                                                                   *
//         File    : WoiCustomWindow.cpp                             *
//         Author  : Nazuha26                                        *
//                                                                   *
//         Copyright (c) 2025 by Nazuha26                            *
//         All bugs are my own. Handle with care.                    *
//                                                                   *
//********************************************************************

//         Created by Nazuha26 in CLion
//         Date: 13.08.2025
//         Time: 18:25:34

#ifndef WINVER
#define WINVER 0x0A00   // WIN10+
#endif

#include "WoiCustomWindow.h"
#include <QApplication>
#include <QLabel>
#include <qlineedit.h>
#include <QMenuBar>
#include <QPainter>
#include <QPushButton>
#include <QVBoxLayout>
#include <QWindow>
#include <uxtheme.h>
#include <windowsx.h>
#include <winuser.h>
#include "core/Icons.h"

#ifdef Q_OS_WIN

bool WoiCustomWindow::isDwmEnabled() {
    BOOL e = FALSE;
    DwmIsCompositionEnabled(&e);
    return e;
}



inline qreal windowDevicePixelRatio(const QWidget* w)
{
    if (w->windowHandle())
        return w->devicePixelRatio();
    return qApp->devicePixelRatio(); // fallback
}



QPoint globPhys2GlobLogicQt(const QWidget* w, const LPARAM lParam)
{
    const qreal s = windowDevicePixelRatio(w);
    const int px = GET_X_LPARAM(lParam);
    const int py = GET_Y_LPARAM(lParam);
    return { static_cast<int>(std::round(px / s)), static_cast<int>(std::round(py / s)) };
}



static inline bool pointInWidgetLogical(const QWidget* w, const QPoint& globalLogical)
{
    if (!w || !w->isVisible())
        return false;
    const QRect r = QRect(w->mapToGlobal(QPoint(0, 0)), w->size());
    return r.contains(globalLogical);
}


/**
 * Returns the height of the little space at the top of the window used to
 * resize the window.
 *
 * See also NonClientIslandWindow::_GetResizeHandleHeight() here:
 * https://github.com/microsoft/terminal/blob/main/src/cascadia/WindowsTerminal/NonClientIslandWindow.cpp
 */
int WoiCustomWindow::getTopResizeHandleHeight(HWND hwnd) {
    const uint dpi = GetDpiForWindow(hwnd);

    // there isn't a SM_CYPADDEDBORDER for the Y axis
    return GetSystemMetricsForDpi(SM_CXPADDEDBORDER, dpi)
         + GetSystemMetricsForDpi(SM_CYSIZEFRAME, dpi);
}



bool WoiCustomWindow::isInEmptyTitlebarArea(const LPARAM lParam) const {
    const QPoint g = globPhys2GlobLogicQt(this, lParam);

    // cursor inside titlebar?
    if (!pointInWidgetLogical(titleBarWidget_, g))
        return false;

    // exclude interactive areas from "exclusions_" widgets list
    for (const QWidget* w : exclusions_) {
        if (pointInWidgetLogical(w, g))
            return false;
    }
    return true;
}
#endif


void WoiCustomWindow::addWidget2titleBar(QWidget* w, const int index) {
    if (!w) return;

    titleBarContentBox_->insertWidget(index, w);

    exclusions_.append(w);

    titleBarWidget_->layout()->activate();
}


void WoiCustomWindow::setupMenuBar(QMenuBar* mb) const
{
    mb->setFixedHeight(TITLEBAR_HEIGHT);
    mb->setSizePolicy(QSizePolicy::Minimum, QSizePolicy::Fixed);

#ifdef Q_OS_WIN
    mb->setNativeMenuBar(false);   // Windows does not have native menu bar
#endif

    // Computing vertical padding for menu bar
    const int textH = QFontMetrics(mb->font()).height();
    int vpad = (TITLEBAR_HEIGHT - textH) / 2;
    if (vpad < 2) vpad = 4;       // fallback value

    vpad &= ~1;  // make even

    const QString qss = QStringLiteral(
        "QMenuBar::item{ padding: %1px 16px; }"
    ).arg(QString::number(vpad));
    mb->setStyleSheet(qss);
}
void WoiCustomWindow::setMenuBar(QMenuBar* mb)
{
    addWidget2titleBar(mb, 0);
    setupMenuBar(mb);
}



WoiCustomWindow::WoiCustomWindow(QWidget* child, QWidget* parent) : QFrame(parent),
                                                              root_(new QVBoxLayout(this)),
                                                              titleBarWidget_(new QWidget()),
                                                              titleBarRootBox_(new QHBoxLayout()),
                                                              titleBarContentBox_(new QHBoxLayout()),
                                                              minBtn_(new QPushButton),
                                                              maxBtn_(new QPushButton),
                                                              closeBtn_(new QPushButton),
                                                              lblTitle_(new QLabel("Window title"))
{
#ifdef Q_OS_WIN
    setWindowFlags(Qt::Window);
#endif

    root_->setSpacing(0);
    root_->setContentsMargins(0,0,0,0);
    root_->addWidget(titleBarWidget_);

    titleBarContentBox_->setSpacing(5);
    titleBarContentBox_->setContentsMargins(0,0,0,0);
    titleBarRootBox_->addLayout(titleBarContentBox_);

    titleBarWidget_->setLayout(titleBarRootBox_);
    titleBarRootBox_->setSpacing(0);
    titleBarRootBox_->setContentsMargins(0,0,0,0);

    titleBarContentBox_->addStretch();
    titleBarContentBox_->addWidget(lblTitle_);
    titleBarContentBox_->addStretch();

    auto* btnLayout = new QHBoxLayout();
    titleBarRootBox_->addLayout(btnLayout);

    minBtn_->setObjectName("btnMinimize");
    minBtn_->setFixedSize(winBtnSize);
    minBtn_->setToolTip("Minimize");
    btnLayout->addWidget(minBtn_);

    maxBtn_->setObjectName("btnMaximize");
    maxBtn_->setFixedSize(winBtnSize);
    maxBtn_->setToolTip("Maximize");
    btnLayout->addWidget(maxBtn_);

    closeBtn_->setObjectName("btnClose");
    closeBtn_->setFixedSize(winBtnSize);
    closeBtn_->setToolTip("Close");
    btnLayout->addWidget(closeBtn_);

    exclusions_ = { minBtn_, maxBtn_, closeBtn_ };

    /* --- Content widget --- */
    contentWidget_ = child ? child : new QWidget(this);
    if (contentWidget_->parent() != this)
        contentWidget_->setParent(this);
    root_->addWidget(contentWidget_);
    contentWidget_->setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Expanding);

#ifdef Q_OS_WIN
    connect(minBtn_, &QPushButton::clicked, this, [this] {
        HWND hwnd = reinterpret_cast<HWND>(winId());
        if (hwnd) SendMessageW(hwnd, WM_SYSCOMMAND, SC_MINIMIZE, 0);
    });

    connect(maxBtn_, &QPushButton::clicked, this, [this] {
        HWND hwnd = reinterpret_cast<HWND>(winId());
        if (hwnd)
        {
            if (IsZoomed(hwnd))
                SendMessageW(hwnd, WM_SYSCOMMAND, SC_RESTORE, 0);
            else
                SendMessageW(hwnd, WM_SYSCOMMAND, SC_MAXIMIZE, 0);
        }
    });

    connect(closeBtn_, &QPushButton::clicked, this, [this] {
            HWND hwnd = reinterpret_cast<HWND>(winId());
            if (hwnd) SendMessageW(hwnd, WM_SYSCOMMAND, SC_CLOSE, 0);
    });
#endif
}

void WoiCustomWindow::showEvent(QShowEvent* event)
{
    QWidget::showEvent(event);

#ifdef Q_OS_WIN
    HWND hwnd = reinterpret_cast<HWND>(winId());

    const QPalette pal = qApp->palette();

    const QColor hcolor = pal.highlight().color();
    const COLORREF borderColor = RGB(hcolor.red(), hcolor.green(), hcolor.blue());
    DwmSetWindowAttribute(hwnd, DWMWA_BORDER_COLOR, &borderColor, sizeof(borderColor));

    // repaint window:
    SetWindowPos(hwnd, nullptr, 0,0,0,0,
                 SWP_NOMOVE | SWP_NOSIZE | SWP_NOZORDER | SWP_FRAMECHANGED);
#endif
}

bool WoiCustomWindow::nativeEvent(const QByteArray& eventType, void* message, qintptr* result)
{
#ifdef Q_OS_WIN
    const MSG* msg = static_cast<MSG*>(message);
    HWND hwnd = msg->hwnd;

    if (msg->message) {

        // If DWM composition is enabled, let DwmDefWindowProc handle the message first.
        LRESULT lret = 0;
        if (isDwmEnabled() && DwmDefWindowProc(hwnd, msg->message, msg->wParam, msg->lParam, &lret)) {
            *result = lret;
            return true;
        }

        switch (msg->message)
        {
            // Remove the standard non-client area (title bar) while preserving system shadows and resize borders.
            case WM_NCCALCSIZE: {
                if (msg->wParam != TRUE) break;

                NCCALCSIZE_PARAMS* p = reinterpret_cast<NCCALCSIZE_PARAMS*>(msg->lParam);
                const int originalTop = p->rgrc[0].top;

                // Call the default window proc to compute system shadow/borders and sizing edges (left, right and bottom).
                const LRESULT def = DefWindowProcW(hwnd, msg->message, msg->wParam, msg->lParam);
                Q_UNUSED(def);

                p->rgrc[0].top = originalTop;

                if (IsZoomed(hwnd))
                    p->rgrc[0].top += getTopResizeHandleHeight(hwnd);

                *result = 0;
                return true;
            }

            // Custom hit test for resize and drag
            case WM_NCHITTEST: {
                // Use default process at first
                const LRESULT def = DefWindowProcW(hwnd, msg->message, msg->wParam, msg->lParam);
                if (def != HTCLIENT)
                    { *result = def; return true; }

                // Convert cursor to window coords
                RECT rw { };
                GetWindowRect(hwnd, &rw);
                const QPoint globPhysPos(GET_X_LPARAM(msg->lParam), GET_Y_LPARAM(msg->lParam));
                const int x = globPhysPos.x() - rw.left;
                const int y = globPhysPos.y() - rw.top;

                const bool thick = (GetWindowLongW(hwnd, GWL_STYLE) & WS_THICKFRAME) != 0;    // window has a sizing border?
                if (thick && y < getTopResizeHandleHeight(hwnd)) {
                    *result = HTTOP;
                    return true;
                }

                // Drag zone (title bar), only in its empty areas
                if (isInEmptyTitlebarArea(msg->lParam)) {
                    *result = HTCAPTION;
                    return true;
                }

                *result = HTCLIENT;
                return true;
            }

            // Handle DPI scaling changes
            case WM_DPICHANGED: {
                const LRESULT lr = DefWindowProcW(hwnd, msg->message, msg->wParam, msg->lParam);

                // if window is maximized and DPI/scaling changed, then Windows
                // does not send a subsequent WM_SIZE message,
                // which depend on scale factor, are not updated
                if (IsZoomed(hwnd)) {
                    const RECT* r = reinterpret_cast<RECT*>(msg->lParam);       // preferred size for new dpi
                    SendMessageW(hwnd, WM_SIZE, SIZE_MAXIMIZED,
                                 MAKELPARAM(r->right - r->left, r->bottom - r->top));
                }
                *result = lr;
                return true;
            }

            // Change win buttons style
            case WM_SIZE: {
                switch (msg->wParam)
                {
                    case SIZE_MAXIMIZED:
                        if (maxBtn_)
                        {
                            maxBtn_->setIcon(QIcon(Icons::path(Icons::RestoreDT)));
                            maxBtn_->setToolTip("Restore");
                        }
                    break;
                    case SIZE_RESTORED:
                        if (maxBtn_)
                        {
                            maxBtn_->setIcon(QIcon(Icons::path(Icons::MaximizeDT)));
                            maxBtn_->setToolTip("Maximize");
                        }
                    break;
                    default:
                        break;
                }
                break;
            }

            default:
                break;
        }
    }
#endif
    return QWidget::nativeEvent(eventType, message, result);
}