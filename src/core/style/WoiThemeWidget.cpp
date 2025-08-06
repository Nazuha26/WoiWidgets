//********************************************************************
//                                                                   *
//         Project : WoiWidgets                                      *
//         File    : WoiThemeWidget.cpp                              *
//         Author  : Nazuha26                                        *
//                                                                   *
//         Copyright (c) 2025 by Nazuha26                            *
//         All bugs are my own. Handle with care.                    *
//         Copyright last updated on 05.08.2025 at (09:43:17)        *
//                                                                   *
//********************************************************************

//         Created by Nazuha26 in CLion
//         Date: 05.08.2025
//         Time: 09:43:17

#include "WoiThemeWidget.h"



WoiThemeWidget::WoiThemeWidget(QWidget* parent) : QWidget(parent)
{ connect(WoiThemeEngine::instance(), &WoiThemeEngine::themeChanged,
    this, &WoiThemeWidget::onThemeChanged); }



void WoiThemeWidget::onThemeChanged() { applyTheme(WoiThemeEngine::instance()->currentTheme()); }