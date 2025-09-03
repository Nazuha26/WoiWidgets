//********************************************************************
//                                                                   *
//         Project : WoiWidgets                                      *
//         File    : WoiThemeEngine.cpp                              *
//         Author  : Nazuha26                                        *
//                                                                   *
//         Copyright (c) 2025 by Nazuha26                            *
//         All bugs are my own. Handle with care.                    *
//         Copyright last updated on 06.08.2025 at (21:02:33)        *
//                                                                   *
//********************************************************************

//         Created by Nazuha26 in CLion
//         Date: 04.08.2025
//         Time: 20:18:08

#include "../core/WoiThemeEngine.h"

#include <QApplication>
#include <QPalette>

using CR = QPalette::ColorRole;

static constexpr CR usingRoles[] = {
    CR::Window,
    CR::WindowText,
    CR::Base,
    CR::Text,
    CR::AlternateBase,
    CR::PlaceholderText,
    CR::Button,
    CR::ButtonText,
    CR::BrightText,
    CR::Light,
    CR::Midlight,
    CR::Mid,
    CR::Dark,
    CR::Shadow,
    CR::Highlight,
    CR::HighlightedText,
    CR::Link,
    CR::LinkVisited
};




void WoiThemeEngine::buildInactivePalette(QPalette& palette, const int darkerPercent)
{
    for (const CR role : usingRoles)
    {
        QColor activeColor = palette.color(QPalette::Active, role);
        palette.setColor(QPalette::Inactive, role, activeColor.darker(darkerPercent));
    }
}



void WoiThemeEngine::buildDisabledPalette(QPalette& palette, const int darkerPercent)
{
    for (const CR role : usingRoles)
    {
        QColor activeColor = palette.color(QPalette::Active, role);

        const int gray = qGray(activeColor.rgb()); // 0–255
        QColor grayColor(gray, gray, gray, activeColor.alpha());

        grayColor = grayColor.darker(darkerPercent);

        palette.setColor(QPalette::Disabled, role, grayColor);
    }
}



void WoiThemeEngine::applyLightTheme()
{
    /* ----- QPALETTE SET UP ----- */
    QPalette palette = qApp->palette();

    palette.setColor(QPalette::Active, QPalette::Window,           QColor(0xF5F5F7));
    palette.setColor(QPalette::Active, QPalette::WindowText,       QColor(0x1A1A1A));
    palette.setColor(QPalette::Active, QPalette::Base,             QColor(0xFFFFFF));
    palette.setColor(QPalette::Active, QPalette::Text,             QColor(0x202124));
    palette.setColor(QPalette::Active, QPalette::AlternateBase,    QColor(0xF0F0F2));
    palette.setColor(QPalette::Active, QPalette::PlaceholderText,  QColor(0x9AA0A6));

    palette.setColor(QPalette::Active, QPalette::Button,           QColor(0xE5E7EB));
    palette.setColor(QPalette::Active, QPalette::ButtonText,       QColor(0x1A1A1A));
    palette.setColor(QPalette::Active, QPalette::BrightText,       QColor(0xD00000));

    palette.setColor(QPalette::Active, QPalette::Light,            QColor(0xFFFFFF));
    palette.setColor(QPalette::Active, QPalette::Midlight,         QColor(0xD1D5DB));
    palette.setColor(QPalette::Active, QPalette::Mid,              QColor(0xC0C4C9));
    palette.setColor(QPalette::Active, QPalette::Dark,             QColor(0x9CA3AF));
    palette.setColor(QPalette::Active, QPalette::Shadow,           QColor(0x6B7280));

    palette.setColor(QPalette::Active, QPalette::Highlight,        QColor(0x5e49b3));
    palette.setColor(QPalette::Active, QPalette::HighlightedText,  QColor(0xFFFFFF));

    palette.setColor(QPalette::Active, QPalette::Link,             QColor(0x2563EB));
    palette.setColor(QPalette::Active, QPalette::LinkVisited,      QColor(0x7C3AED));

    /* ===== INACTIVE ===== */
    buildInactivePalette(palette, 105);

    /* ===== DISABLED ===== */
    buildDisabledPalette(palette, 150);

    /* --- TOOLTIP --- */
    palette.setColor(QPalette::Active,   QPalette::ToolTipBase, QColor(0xF9FAFB));
    palette.setColor(QPalette::Active,   QPalette::ToolTipText, QColor(0x1A1A1A));
    palette.setColor(QPalette::Inactive, QPalette::ToolTipBase, QColor(0xF9FAFB));
    palette.setColor(QPalette::Inactive, QPalette::ToolTipText, QColor(0x1A1A1A));
    palette.setColor(QPalette::Disabled, QPalette::ToolTipBase, QColor(0xF9FAFB));
    palette.setColor(QPalette::Disabled, QPalette::ToolTipText, QColor(0x1A1A1A));

    qApp->setPalette(palette);
}




void WoiThemeEngine::applyDarkTheme()
{
    /* ----- QPALETTE SET UP ----- */
    QPalette palette = qApp->palette();

    palette.setColor(QPalette::Active, QPalette::Window,           QColor(0x2F3138));
    palette.setColor(QPalette::Active, QPalette::WindowText,       QColor(0xE6E8EA));
    palette.setColor(QPalette::Active, QPalette::Base,             QColor(0x3A3C44));
    palette.setColor(QPalette::Active, QPalette::Text,             QColor(0xC9CCD1));
    palette.setColor(QPalette::Active, QPalette::AlternateBase,    QColor(0x45474F));
    palette.setColor(QPalette::Active, QPalette::PlaceholderText,  QColor(0x9AA0A6));

    palette.setColor(QPalette::Active, QPalette::Button,           QColor(0x2b2d30));
    palette.setColor(QPalette::Active, QPalette::ButtonText,       QColor(0xE6E8EA));
    palette.setColor(QPalette::Active, QPalette::BrightText,       QColor(0xFF5A5A));

    palette.setColor(QPalette::Active, QPalette::Light,            QColor(0x7E848E));
    palette.setColor(QPalette::Active, QPalette::Midlight,         QColor(0x646A74));
    palette.setColor(QPalette::Active, QPalette::Mid,              QColor(0x2B2F36));
    palette.setColor(QPalette::Active, QPalette::Dark,             QColor(0x1F2329));
    palette.setColor(QPalette::Active, QPalette::Shadow,           QColor(0x0F1114));

    palette.setColor(QPalette::Active, QPalette::Highlight,        QColor(0x5e49b3));
    palette.setColor(QPalette::Active, QPalette::HighlightedText,  QColor(0xFFFFFF));

    palette.setColor(QPalette::Active, QPalette::Link,             QColor(0x60A5FA));
    palette.setColor(QPalette::Active, QPalette::LinkVisited,      QColor(0xA78BFA));



    /* ===== INACTIVE ===== */
    buildInactivePalette(palette, 110);

    /* ===== DISABLED ===== */
    buildDisabledPalette(palette, 150);



    /* --- TOOLTIP --- */
    palette.setColor(QPalette::Active, QPalette::ToolTipBase,      QColor(0x27292E)); // tool tips are not active windows
    palette.setColor(QPalette::Active, QPalette::ToolTipText,      QColor(0xE6E8EA)); // tool tips are not active windows
    palette.setColor(QPalette::Inactive, QPalette::ToolTipBase,      QColor(0x27292E)); // tool tips are not active windows
    palette.setColor(QPalette::Inactive, QPalette::ToolTipText,      QColor(0xE6E8EA)); // tool tips are not active windows
    palette.setColor(QPalette::Disabled, QPalette::ToolTipBase,      QColor(0x27292E)); // tool tips are not active windows
    palette.setColor(QPalette::Disabled, QPalette::ToolTipText,      QColor(0xE6E8EA)); // tool tips are not active windows

    qApp->setPalette(palette);
}