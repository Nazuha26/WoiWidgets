//********************************************************************
//                                                                   *
//         Project : WoiWidgets                                      *
//         File    : WoiThemeEngine.h                                *
//         Author  : Nazuha26                                        *
//                                                                   *
//         Copyright (c) 2025 by Nazuha26                            *
//         All bugs are my own. Handle with care.                    *
//         Copyright last updated on 04.08.2025 at (20:18:09)        *
//                                                                   *
//********************************************************************

//         Created by Nazuha26 in CLion
//         Date: 04.08.2025
//         Time: 20:18:08

#ifndef WOITHEMEENGINE_H
#define WOITHEMEENGINE_H

#define ThemeEngine WoiThemeEngine::instance()

#include <QBrush>
#include <QColor>
#include <QObject>

class WoiThemeEngine final : public QObject {
    Q_OBJECT

public:

    struct WoiTheme {
        struct WindowFrameStyle {
            /* --- colors --- */
            QColor fgColor;
            QColor fgOutlineColor;
            QBrush outlineBrush;
        } windowFrame;

        struct ResizePreviewStyle {
            QColor color;
        } resizePreview;

        struct ShadowOverlayWindowStyle {
            QColor color;
            qreal  topAlpha;
            qreal  bottomAlpha;
        } shadowOverlay;

        struct PushButtonStyle {
            /* --- colors --- */
            QColor fgColor;
            QColor normalColor;
            QColor hoverColor;
            QColor pressColor;
        } pushButton;
    };



    static WoiTheme loadLightThemePreset();
    static WoiTheme loadDarkThemePreset();
    void            applyTheme(const WoiTheme& theme);  // apply and update current theme

    static WoiThemeEngine* instance();
    WoiTheme& currentTheme() { return theme_; };
    void updateTheme() { emit themeChanged(); };

private:
    explicit WoiThemeEngine(QObject* parent = nullptr);
    ~WoiThemeEngine() override = default;

    WoiTheme theme_;

signals:
    void themeChanged();

};



#endif //WOITHEMEENGINE_H
