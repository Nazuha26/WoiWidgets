//********************************************************************
//                                                                   *
//         Project : WoiWidgets                                      *
//         File    : WoiTitleBarButton.h                             *
//         Author  : Nazuha26                                        *
//                                                                   *
//         Copyright (c) 2025 by Nazuha26                            *
//         All bugs are my own. Handle with care.                    *
//         Copyright last updated on 25.07.2025 at (09:23:45)        *
//                                                                   *
//********************************************************************

//
// Created by Nazar on 23.07.2025.
//

#ifndef WOITITLEBARBUTTON_H
#define WOITITLEBARBUTTON_H

#include <QSvgRenderer>
#include <QWidget>

#include "../core/WoiMacros.h"



class WoiTitleBarButton final : public QWidget {
    Q_OBJECT

public:
    /* --- CONSTRUCTORS --- */
    explicit WoiTitleBarButton(const QString& svgPath, QWidget* parent = nullptr);
    explicit WoiTitleBarButton(const QString& svgPath, QColor normalColor, QColor hoverColor, QColor pressColor,
                               float contentScaleFactor = 0.7f, QWidget* parent = nullptr);
    ~WoiTitleBarButton();

    /* --- GETTERS --- */
    [[nodiscard]] QColor  normalColor() const { return normalColor_; }
    [[nodiscard]] QColor  hoverColor() const { return hoverColor_; }
    [[nodiscard]] QColor  pressColor() const { return pressColor_; }
    [[nodiscard]] float   contentScaleFactor() const { return contentScaleFactor_; }
    [[nodiscard]] QString svgPath() const { return svgPath_; }

    /* --- SETTERS --- */
    void setNormalColor(const QColor& color);
    void setHoverColor(const QColor& color);
    void setPressColor(const QColor& color);
    void setContentScaleFactor(const float v) { STYLE_SETTER(contentScaleFactor_, v); }
    void setSvgIcon(const QString& path);

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void enterEvent(QEnterEvent* event) override;

    [[nodiscard]] QSize sizeHint() const override;

private:
    enum State { NORMAL, HOVER, PRESS };



    QColor normalColor_;
    QColor hoverColor_;
    QColor pressColor_;

    float contentScaleFactor_; // scale relative size (0.0 — 1.0)

    QString svgPath_; // svg icon

    /* --- LOCAL --- */
    State         currentState_ = NORMAL;
    QColor        currentColor_ = normalColor_;
    QSvgRenderer* svgRenderer_  = nullptr;

    /* --- CORE --- */
    void updateCurrentColor();

signals:
    void clicked();
};


#endif //WOITITLEBARBUTTON_H