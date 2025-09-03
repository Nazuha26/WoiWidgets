//********************************************************************
//                                                                   *
//         File    : Icons.cpp                                       *
//         Author  : Nazuha26                                        *
//                                                                   *
//         Copyright (c) 2025 by Nazuha26                            *
//         All bugs are my own. Handle with care.                    *
//                                                                   *
//********************************************************************

//
// Created by Nazar on 21.07.2025.
//

#include "../core/Icons.h"

#include <QString>

QString Icons::path(const Type type)
{
    switch (type)
    {
        case MinimizeDT: return ":/res/woi_dark/icons/24x24/minimize.png";
        case RestoreDT: return ":/res/woi_dark/icons/24x24/restore.png";
        case MaximizeDT: return ":/res/woi_dark/icons/24x24/maximize.png";
        case CloseDT: return ":/res/woi_dark/icons/24x24/close.png";
        case ArrowUpDT: return ":/res/woi_dark/icons/24x24/arrow_up.png";
        case ArrowDownDT: return ":/res/woi_dark/icons/24x24/arrow_down.png";
        case ArrowLeftDT: return ":/res/woi_dark/icons/24x24/arrow_left.png";
        case ArrowRightDT: return ":/res/woi_dark/icons/24x24/arrow_right.png";

        case TestIcon: return ":/woiCom/icons/test_24.png";
        case Woi20: return ":/woiCom/icons/woi_20.png";
        case Woi32: return ":/woiCom/icons/woi_32.png";

        default: return { };
    }
}