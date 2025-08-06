//********************************************************************
//                                                                   *
//         Project : WoiWidgets                                      *
//         File    : Icons.cpp                                       *
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

#include "Icons.h"

QString Icons::path(const Type type)
{
    switch (type)
    {
        case Test: return ":/icons/test.svg";
        case Expand: return ":/icons/expand.svg";
        case Collapse: return ":/icons/collapse.svg";
        case Minimize: return ":/icons/minimize.svg";
        case Maximize: return ":/icons/maximize.svg";
        case Restore: return ":/icons/restore.svg";
        case Close: return ":/icons/close.svg";

        default: return { };
    }
}