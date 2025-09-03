//********************************************************************
//                                                                   *
//         File    : WoiCustomWindow.h                               *
//         Author  : Nazuha26                                        *
//                                                                   *
//         Copyright (c) 2025 by Nazuha26                            *
//         All bugs are my own. Handle with care.                    *
//                                                                   *
//********************************************************************

//         Created by Nazuha26 in CLion
//         Date: 13.08.2025
//         Time: 18:25:34

#ifndef CUSTOMWINDOW_H
#define CUSTOMWINDOW_H

#include <QFrame>
#include <QHBoxLayout>
#include <QWidget>

#ifdef Q_OS_WIN
#include <dwmapi.h>
#include <windows.h>
#endif

#define TITLEBAR_HEIGHT 32

class QPushButton;
class QLabel;
class QHBoxLayout;
class QVBoxLayout;
class QMenuBar;

class WoiCustomWindow : public QFrame {
    Q_OBJECT
public:
    explicit WoiCustomWindow(QWidget* child = nullptr, QWidget *parent = nullptr);
    void addWidget2titleBar(QWidget* w, int index = -1);
    void setMenuBar(QMenuBar* mb);

    inline void setTitleBarSpacing(const int v) const
    { if (titleBarContentBox_) titleBarContentBox_->setSpacing(v); }

protected:
    void showEvent(QShowEvent* event) override;
    bool nativeEvent(const QByteArray& eventType, void* message, qintptr* result) override;

private:
#ifdef Q_OS_WIN
    static bool isDwmEnabled();
    static int getTopResizeHandleHeight(HWND hwnd);
    [[nodiscard]] bool isInEmptyTitlebarArea(LPARAM lParam) const;
#endif

    void setupMenuBar(QMenuBar* mb) const;

    QLabel* lblTitle_;
    QHBoxLayout* titleBarRootBox_;
    QWidget* titleBarWidget_;
    QVBoxLayout* root_;
    QWidget* contentWidget_;
    QPushButton* minBtn_;
    QPushButton* maxBtn_;
    QPushButton* closeBtn_;
    QHBoxLayout* titleBarContentBox_;

    QList<QWidget*> exclusions_;    // widgets list of holes in titlebar

    QSize winBtnSize = QSize(28, TITLEBAR_HEIGHT);   // default
};


#endif //CUSTOMWINDOW_H
