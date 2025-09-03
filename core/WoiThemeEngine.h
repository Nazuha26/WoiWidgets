//********************************************************************
//                                                                   *
//         File    : WoiThemeEngine.h                                *
//         Author  : Nazuha26                                        *
//                                                                   *
//         Copyright (c) 2025 by Nazuha26                            *
//         All bugs are my own. Handle with care.                    *
//                                                                   *
//********************************************************************

//         Created by Nazuha26 in CLion
//         Date: 04.08.2025
//         Time: 20:18:08

#ifndef WOITHEMEENGINE_H
#define WOITHEMEENGINE_H

class QPalette;

class WoiThemeEngine final {

public:
    static void applyLightTheme();
    static void applyDarkTheme();

private:
    explicit WoiThemeEngine() = default;
    ~WoiThemeEngine() = default;

    /* --- CORE --- */
    static void buildInactivePalette(QPalette& palette, int darkerPercent);
    static void buildDisabledPalette(QPalette& palette, int darkerPercent);

};



#endif //WOITHEMEENGINE_H
