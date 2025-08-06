//********************************************************************
//                                                                   *
//         Project : WoiWidgets                                      *
//         File    : WoiPushButton.h                                 *
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

#ifndef WOIPUSHBUTTON_H
#define WOIPUSHBUTTON_H

#include <QSvgRenderer>
#include <QWidget>

#include "../core/WoiMacros.h"
#include "../core/style/WoiThemeWidget.h"

enum class ContentPosition {
    LEFT,
    CENTER,
    RIGHT
};

class WoiPushButton final : public WoiThemeWidget {
    Q_OBJECT

public:

    /* --- GETTERS --- */
    [[nodiscard]] const QString& text() const { return text_; }
    [[nodiscard]] const QString& svgIconPath() const { return svgIconPath_; }

    [[nodiscard]] const QColor& fgColor() const { return fgColor_; }
    [[nodiscard]] const QColor& normalColor() const { return normalColor_; }
    [[nodiscard]] const QColor& hoverColor() const { return hoverColor_; }
    [[nodiscard]] const QColor& pressColor() const { return pressColor_; }

    [[nodiscard]] ContentPosition iconPos() const { return iconPos_; }
    [[nodiscard]] ContentPosition textPos() const { return textPos_; }

    [[nodiscard]] int iconSize() const { return iconSize_; }
    [[nodiscard]] float textSize() const { return textSize_; }

    [[nodiscard]] int iconSpacing() const { return iconSpacing_; }
    [[nodiscard]] int contentMargin() const { return contentMargin_; }
    [[nodiscard]] int cornerCut() const { return cornerCut_; }


    /* --- SETTERS --- */
    void setText(const QString& text);
    void setSvgIcon(const QString& path);

    //void setForegroundColor(const QColor& color) { STYLE_SETTER(fgColor_, color); }
    //void setNormalColor(const QColor& color);
    //void setHoverColor(const QColor& color);
    //void setPressColor(const QColor& color);

    void setIconPosition(const ContentPosition pos) { iconPos_ = pos; update(); }
    void setTextPosition(const ContentPosition pos) { textPos_ = pos; update(); }

    void setIconSize(int v);
    void setTextSize(float v);

    void setIconSpacing(int v);
    void setContentMargin(int v);
    void setCornerCut(const int v) { STYLE_SETTER(cornerCut_, v); }

    /* --- PUBLIC CORE --- */

    explicit WoiPushButton(const QString& svgIconPath, const QString& text, QWidget* parent = nullptr);
    ~WoiPushButton() = default;

protected:
    void paintEvent(QPaintEvent* event) override;
    void mousePressEvent(QMouseEvent* event) override;
    void mouseReleaseEvent(QMouseEvent* event) override;
    void leaveEvent(QEvent* event) override;
    void enterEvent(QEnterEvent* event) override;

    [[nodiscard]] QSize sizeHint() const override;
    [[nodiscard]] QSize minimumSizeHint() const override;

    void applyTheme(const WoiTheme& theme) override;


private:
    enum State { NORMAL, HOVER, PRESS };

    QString text_;
    QString svgIconPath_; // path to svg icon

    /* ----- STYLE ----- */
    QColor fgColor_;
    QColor normalColor_;
    QColor hoverColor_;
    QColor pressColor_;

    ContentPosition iconPos_; // left or right
    ContentPosition textPos_;
    int             iconSize_;
    float           textSize_;
    int             iconSpacing_;
    int             contentMargin_; // content margin from each side
    int             cornerCut_;



    /* --- LOCAL --- */
    State         currentState_;
    QColor        currentColor_;
    QSvgRenderer* svgRenderer_ ;

    /* --- CORE --- */
    void updateCurrentColor();
    [[nodiscard]] QSize effectiveContentSize(const QFontMetrics &fm,
                                             bool hasIcon,
                                             bool hasText) const;
    void invalidateGeometry();
    void applyStyle(const WoiTheme::PushButtonStyle& style);
    void syncStyle();

signals:
    void clicked();
};



#endif //WOIPUSHBUTTON_H
