//********************************************************************
//                                                                   *
//         Project : WoiWidgets                                      *
//         File    : WoiMacros.h                                     *
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

#ifndef WOIMACROS_H
#define WOIMACROS_H

#include <QTime>
#include <QDebug>

#define STYLE_SETTER(field, value)                                                \
    do {                                                                           \
        if ((value) == (field)) { /*WOI_DEBUG("Value has not changed");*/ return; } \
        field = (value);                                                             \
        update();                                                                     \
    } while (0)

#if __cplusplus >= 202002L
    #define WOI_DEBUG(msg)                                                          \
        do {                                                                         \
            qDebug() << "[DEBUG] | " << QTime::currentTime().toString("[HH:mm:ss]")   \
            << " | " << __FILE_NAME__ << "::" << __FUNCTION__ << " -> " << msg;        \
        } while (0)

#else
    #define WOI_DEBUG(msg)                                                          \
        do {                                                                         \
            qDebug() << "[DEBUG] | " << QTime::currentTime().toString("[HH:mm:ss]")   \
            << " | " << __FUNCTION__ << " -> " << msg;        \
        } while (0)
#endif


#endif //WOIMACROS_H
