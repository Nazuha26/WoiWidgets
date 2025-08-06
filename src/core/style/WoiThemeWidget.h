//********************************************************************
//                                                                   *
//         Project : WoiWidgets                                      *
//         File    : WoiThemeWidget.h                                *
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

#ifndef WOITHEMEWIDGET_H
#define WOITHEMEWIDGET_H

#include <QWidget>
#include "WoiThemeEngine.h"

using WoiTheme = WoiThemeEngine::WoiTheme;

class WoiThemeWidget : public QWidget {
    Q_OBJECT

public:
    explicit WoiThemeWidget(QWidget* parent = nullptr);
    ~WoiThemeWidget() override = default;

protected:
    virtual void applyTheme(const WoiTheme& theme) = 0;

private slots:
    void onThemeChanged();
};



#endif //WOITHEMEWIDGET_H
