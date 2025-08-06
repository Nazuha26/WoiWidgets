//********************************************************************
//                                                                   *
//         Project : WoiWidgets                                      *
//         File    : WoiTitleBar.h                                   *
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

#ifndef WOITITLEBAR_H
#define WOITITLEBAR_H

#include <QHBoxLayout>
#include <QLabel.h>

class WoiTitleBarButton;



class WoiTitleBar final : public QWidget {
    Q_OBJECT

public:
    void setHeight(int v);

    [[nodiscard]] WoiTitleBarButton* btnCollapse() const { return btnCollapse_; }
    [[nodiscard]] WoiTitleBarButton* btnMinimize() const { return btnMinimize_; }
    [[nodiscard]] WoiTitleBarButton* btnMaximize() const { return btnMaximize_; }
    [[nodiscard]] WoiTitleBarButton* btnClose() const { return btnClose_; }

    [[nodiscard]] QString woiWindowTitle() const { return windowTitle_; }
    void                  setWoiWindowTitle(const QString& text);

    void hideBtnMinimize() const;
    void hideBtnMaximize() const;
    void showBtnMinimize() const;
    void showBtnMaximize() const;


    [[nodiscard]] int visibleWinButtonsCount() const;

    explicit WoiTitleBar(const QString& title = "My window", int height = 36, QWidget* parent = nullptr);
    ~WoiTitleBar();

    /* ----- STYLE CONSTANTS ----- */
    static constexpr int       WIN_BTN_SPACING      = 2;
    static constexpr int       WIN_BTN_MARGIN       = 2;
    static constexpr int       WIN_BTN_HEIGHT       = 28;
    static constexpr int       WIN_BTN_WIDTH        = 28;
    inline static const QColor COMMON_NORMAL_COLOR  = QColor(0, 0, 0, 0);
    inline static const QColor COMMON_HOVER_COLOR   = QColor(0, 0, 0, 70);
    inline static const QColor COMMON_PRESSED_COLOR = QColor(0, 0, 0, 120);
    inline static const QColor CLOSE_NORMAL_COLOR   = QColor(0,0,0,0);
    inline static const QColor CLOSE_HOVER_COLOR    = QColor(0xD84040);
    inline static const QColor CLOSE_PRESSED_COLOR  = QColor(0x8E1616);

protected:
    void mousePressEvent(QMouseEvent* event) override;
    void mouseMoveEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void mouseDoubleClickEvent(QMouseEvent* event) override;

signals:
    void dragStarted(const QPoint& globalPos);
    void dragging(const QPoint& globalPos);
    void dragFinished();

    void doubleClicked();

private:
    void updateMask();

    /* ----- UI ----- */
    QHBoxLayout* mainContainer_;
    QHBoxLayout* winBtnsContainer_;

    QLabel* lblWindowTitle_;

    WoiTitleBarButton* btnCollapse_ = nullptr;
    WoiTitleBarButton* btnMinimize_;
    WoiTitleBarButton* btnMaximize_;
    WoiTitleBarButton* btnClose_;
    /* -------------- */



    /* ----- PARAMS ----- */
    QString windowTitle_;
    int     titleBarHeight_;
};



#endif //WOITITLEBAR_H