//********************************************************************
//                                                                   *
//         Project : WoiWidgets                                      *
//         File    : Icons.h                                         *
//         Author  : Nazuha26                                        *
//                                                                   *
//         Copyright (c) 2025 by Nazuha26                            *
//         All bugs are my own. Handle with care.                    *
//         Copyright last updated on 25.07.2025 at (09:23:45)        *
//                                                                   *
//********************************************************************

//
// Created by Nazar on 21.07.2025.
//

#ifndef ICONS_H
#define ICONS_H

#include <QString>

class Icons {
public:
    enum Type
    {
        Test,
        Expand, Collapse,
        Minimize,
        Maximize, Restore,
        Close
    };

    static QString path(Type type);
};



#endif //ICONS_H
