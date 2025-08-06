//********************************************************************
//                                                                   *
//         Project : WoiWidgets                                      *
//         File    : WoiTitleBar.cpp                                 *
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

#include "WoiTitleBar.h"
#include "WoiTitleBarButton.h"

#include <QMouseEvent>
#include <QPainter>
#include <QPainterPath>

#include "../core/Icons.h"



WoiTitleBar::WoiTitleBar(const QString& title, const int height, QWidget* parent) : QWidget(parent),
                                            windowTitle_(title),
                                            titleBarHeight_(height)
{

    setObjectName("TitleBar");
    setAttribute(Qt::WA_TranslucentBackground);
    setSizePolicy(QSizePolicy::Expanding, QSizePolicy::Fixed);
    setFixedHeight(titleBarHeight_);

    mainContainer_ = new QHBoxLayout();
    mainContainer_->setSpacing(0);
    mainContainer_->setContentsMargins(0, 0, 0, 0);

    lblWindowTitle_ = new QLabel(windowTitle_);

    winBtnsContainer_ = new QHBoxLayout();
    winBtnsContainer_->setSpacing(WIN_BTN_SPACING);
    winBtnsContainer_->setContentsMargins(WIN_BTN_MARGIN,WIN_BTN_MARGIN,WIN_BTN_MARGIN,WIN_BTN_MARGIN);
    winBtnsContainer_->setAlignment(Qt::AlignTop);

    /* --- BUTTONS SETUP --- */
    //btnCollapse_ = new WoiTitleBarButton(Icons::path(Icons::Expand));
    btnMinimize_ = new WoiTitleBarButton(Icons::path(Icons::Minimize));
    btnMaximize_ = new WoiTitleBarButton(Icons::path(Icons::Maximize));
    btnClose_    = new WoiTitleBarButton(Icons::path(Icons::Close), CLOSE_NORMAL_COLOR, CLOSE_HOVER_COLOR,
                                         CLOSE_PRESSED_COLOR);

    const QList<WoiTitleBarButton*> titleBtns = { btnMinimize_, btnMaximize_, btnClose_ };
    for (WoiTitleBarButton* btn : titleBtns)
    {
        btn->setFixedSize(WIN_BTN_WIDTH, WIN_BTN_HEIGHT);
        if (btn != btnClose_)
        {
            btn->setNormalColor(COMMON_NORMAL_COLOR);
            btn->setHoverColor(COMMON_HOVER_COLOR);
            btn->setPressColor(COMMON_PRESSED_COLOR);
        }
        winBtnsContainer_->addWidget(btn);
    }
    /* -------------------- */


    mainContainer_->addStretch();
    mainContainer_->addWidget(lblWindowTitle_);     // at center
    mainContainer_->addStretch();

    mainContainer_->addLayout(winBtnsContainer_);

    lblWindowTitle_->setStyleSheet("QLabel {" "    color: #dddddd;" "}");

    setLayout(mainContainer_);
}



WoiTitleBar::~WoiTitleBar() = default;



/* ========== EVENTS ========== */

/* ===== WINDOW DRAG ===== */
void WoiTitleBar::mousePressEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton) return;
    emit dragStarted(event->globalPosition().toPoint());
}



void WoiTitleBar::mouseMoveEvent(QMouseEvent* event)
{
    if (!(event->buttons() & Qt::LeftButton)) return;
    emit dragging(event->globalPosition().toPoint());
}



void WoiTitleBar::mouseReleaseEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton) return;
    emit dragFinished();
}
/* ======================= */



void WoiTitleBar::mouseDoubleClickEvent(QMouseEvent* event)
{
    if (event->button() != Qt::LeftButton) return;
    emit doubleClicked();
}

/* ----------------------------------- */


void WoiTitleBar::setHeight(const int v)
{
    if (v <= 0) return;
    setFixedHeight(v);
    //for (WoiTitleBarButton* btn : { btnCollapse_, btnMinimize_, btnMaximize_, btnClose_ })
    //    btn->setFixedHeight(v);
    //update();
}



void WoiTitleBar::setWoiWindowTitle(const QString& text)
{
    windowTitle_ = text;
    lblWindowTitle_->setText(text);
}



void WoiTitleBar::hideBtnMinimize() const { btnMinimize_->hide(); }
void WoiTitleBar::hideBtnMaximize() const { btnMaximize_->hide(); }
void WoiTitleBar::showBtnMinimize() const { btnMinimize_->show(); }
void WoiTitleBar::showBtnMaximize() const { btnMaximize_->show(); }



int WoiTitleBar::visibleWinButtonsCount() const
{
    int count = 0;
    for (int i = 0; i < winBtnsContainer_->count(); i++)
    {
        QWidget* w = winBtnsContainer_->itemAt(i)->widget();
        if (w && w->isVisible()) count++;
    }
    return count;
}