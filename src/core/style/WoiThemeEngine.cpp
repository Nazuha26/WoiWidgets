//********************************************************************
//                                                                   *
//         Project : WoiWidgets                                      *
//         File    : WoiThemeEngine.cpp                              *
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

#include "WoiThemeEngine.h"

#include <QCoreApplication>

WoiThemeEngine::WoiThemeEngine(QObject* parent) : QObject(parent)
{ theme_ = loadLightThemePreset(); }



WoiThemeEngine::WoiTheme WoiThemeEngine::loadLightThemePreset()
{
    WoiTheme lightTheme;

    /* ----- WINDOW FRAME ----- */
    /* --- colors --- */
    lightTheme.windowFrame.fgColor = QColor(0xAAAAAA);
    lightTheme.windowFrame.fgOutlineColor = QColor(0xCCCCCC);
    lightTheme.windowFrame.outlineBrush = QColor(0x80, 0x80, 0x80, 0x7F);

    /* ----- RESIZE PREVIEW ----- */
    lightTheme.resizePreview.color = QColor(0x00FF00);

    /* ----- SHADOW OVERLAY WINDOW ----- */
    lightTheme.shadowOverlay.color = QColor(0, 0, 0);
    lightTheme.shadowOverlay.topAlpha = 0.2f;
    lightTheme.shadowOverlay.bottomAlpha = 0.7f;

    /* ----- WOI PUSH BUTTON ----- */
    /* --- colors --- */
    lightTheme.pushButton.fgColor = QColor(0xFFFFFF);
    lightTheme.pushButton.normalColor = QColor(0x0000FF);
    lightTheme.pushButton.hoverColor = QColor(0x00FF00);
    lightTheme.pushButton.pressColor = QColor(0xFF0000);

    return lightTheme;
}



WoiThemeEngine::WoiTheme WoiThemeEngine::loadDarkThemePreset()
{
    WoiTheme darkTheme;

    /* ----- WINDOW FRAME ----- */
    /* --- colors --- */
    darkTheme.windowFrame.fgColor = QColor(0xAAAAAA);
    darkTheme.windowFrame.fgOutlineColor = QColor(0xCCCCCC);
    darkTheme.windowFrame.outlineBrush = QColor(0xff, 0x80, 0x80, 0x7F);

    /* ----- RESIZE PREVIEW ----- */
    darkTheme.resizePreview.color = QColor(0xFF0000);

    /* ----- SHADOW OVERLAY WINDOW ----- */
    darkTheme.shadowOverlay.color = QColor(0, 0, 0);
    darkTheme.shadowOverlay.topAlpha = 0.5f;
    darkTheme.shadowOverlay.bottomAlpha = 0.7f;

    /* ----- WOI PUSH BUTTON ----- */
    /* --- colors --- */
    darkTheme.pushButton.fgColor = QColor(0xFFFFFF);
    darkTheme.pushButton.normalColor = QColor(0x00FFFF);
    darkTheme.pushButton.hoverColor = QColor(0xFFFF00);
    darkTheme.pushButton.pressColor = QColor(0xFF00FF);

    return darkTheme;
}



void WoiThemeEngine::applyTheme(const WoiTheme& theme)
{
    theme_ = theme;
    updateTheme();
}



WoiThemeEngine* WoiThemeEngine::instance()
{
    static WoiThemeEngine instance(qApp);
    return &instance;
}