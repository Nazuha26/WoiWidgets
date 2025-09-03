//********************************************************************
//                                                                   *
//         File    : Icons.h                                         *
//         Author  : Nazuha26                                        *
//                                                                   *
//         Copyright (c) 2025 by Nazuha26                            *
//         All bugs are my own. Handle with care.                    *
//                                                                   *
//********************************************************************

//
// Created by Nazar on 21.07.2025.
//

#ifndef ICONS_H
#define ICONS_H

class QString;

class Icons {
public:
    enum Type
    {
        MinimizeDT,
        RestoreDT,
        MaximizeDT,
        CloseDT,
        ArrowUpDT,
        ArrowDownDT,
        ArrowLeftDT,
        ArrowRightDT,

        TestIcon,
        Woi20,
        Woi32,
    };

    static QString path(Type type);
};



#endif //ICONS_H
