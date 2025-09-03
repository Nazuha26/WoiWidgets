//********************************************************************
//                                                                   *
//         File    : WoiStyleMenu.cpp                                *
//         Author  : Nazuha26                                        *
//                                                                   *
//         Copyright (c) 2025 by Nazuha26                            *
//         All bugs are my own. Handle with care.                    *
//                                                                   *
//********************************************************************

//         Created by Nazuha26 in CLion
//         Date: 30.08.2025
//         Time: 18:06:30

#include "WoiStyleMenu.h"
#include <QMenuBar>
#include <QPainter>
#include <QPainterPath>
#include <QScreen>
#include <QStyleOption>

#include "../WoiStyleUtility.h"
#include "../../Icons.h"


#define MC_SPACING 4         // menu components spacing
#define POPUP_PAD 4          // popup padding
#define CR_RADIUS 8          // corner radius
#define SEPARATOR_H_M 4      // horizontal margin
#define M_ICON_SIZE 16, 16   // menu icon size
#define SUBM_ARR_SIZE 8      // submenu arrow size (w & h)
#define BLUR_RADIUS 4.0f
#define POPUP_TINT QColor(0, 0, 0, static_cast<int>(0.4 * 255))
#define POPUP_BC QColor(255, 255, 255, 40)  // border color

void WoiStyleMenu::polishMenu(QMenu* m)
{
    if (!m) return;

    m->setAttribute(Qt::WA_TranslucentBackground);
    m->setWindowFlags(m->windowFlags() | Qt::FramelessWindowHint);
    m->setWindowFlag(Qt::NoDropShadowWindowHint);

    qApp->installEventFilter(new WoiStyleMenuFilter(m));
    m->setContentsMargins(POPUP_PAD,POPUP_PAD,POPUP_PAD,POPUP_PAD);
}



bool WoiStyleMenu::drawMenu(QStyle::PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w)
{
    if (pe == QStyle::PE_PanelMenu || pe == QStyle::PE_FrameMenu)
    {
        // DEFAULT POPUP MENU
        const QRectF r = QRectF(w->rect()).adjusted(0.5, 0.5, -0.5, -0.5); // for border

        QPainterPath path;
        path.addRoundedRect(r, CR_RADIUS, CR_RADIUS);

        p->save();
        p->setRenderHint(QPainter::Antialiasing, true);
        p->setClipPath(path);

        auto it = blurCache_.constFind(w);
        if (it != blurCache_.constEnd())
        {
            if (!it.value().isNull())
                p->drawImage(opt->rect.topLeft(), it.value());
        }
        else   // fallback
        {
            qDebug() << "Not blur for: " << w;
            if (w->inherits("QComboBoxPrivateContainer"))
                qDebug() << "Yes";
        }

        p->fillPath(path, POPUP_TINT);

        p->setClipping(false);
        p->setPen(QPen(POPUP_BC, 1));
        p->setBrush(Qt::NoBrush);
        p->drawRoundedRect(r, CR_RADIUS, CR_RADIUS);
        p->restore();

        return true;
    }
    return false;
}



struct MenuItemRects {
    QRect checkCol;
    QRect iconCol;
    QRect textCol;
    QRect shortcutCol;
};
static bool menuHasAnyIcons(const QMenu* menu) {
    if (!menu) return false;
    for (const QAction* a : menu->actions()) {
        if (!a || !a->isVisible()) continue;
        if (!a->icon().isNull()) return true;
    }
    return false;
}
static MenuItemRects nativeMenuItemRects(const QStyle* style, const QStyleOptionMenuItem* mi, const QWidget* w, bool reserveArrow)
{
    MenuItemRects R { };

    //     ----- DATA -----
    //  CHECK & RADIO COLUMN
    const bool hasCheckCol = mi->checkType != QStyleOptionMenuItem::NotCheckable || mi->menuHasCheckableItems;
    const int checkW = hasCheckCol ? style->pixelMetric(QStyle::PM_IndicatorWidth, mi, w) : 0;

    //  ICON COLUMN
    const QMenu* menu = qobject_cast<const QMenu*>(w);
    const bool needIconCol = menuHasAnyIcons(menu);

    int iconColW = 0;
    if (needIconCol) {
        iconColW = mi->maxIconWidth;
        if (iconColW <= 0)
            iconColW = style->pixelMetric(QStyle::PM_SmallIconSize, mi, w);
    }
    const bool hasIconCol = iconColW > 0;


    //  SHORTCUT COLUMN
    int shortcutW = mi->reservedShortcutWidth;
    if (reserveArrow)
        shortcutW = std::max(shortcutW, SUBM_ARR_SIZE + MC_SPACING);    // reserve a space for submenu arrow icon


    //     ----- CALCULATING -----
    // MENU ANATOMY:
    //      | C | I | TEXT         | SC |
    //  C - Check & Radio btns - left part
    //  I - icon - left part
    //  TEXT - left part
    //  SC - shortcut - right part

    const QRect itemVR = QStyle::visualRect(mi->direction, w ? w->rect() : mi->rect, mi->rect);

    // Left part rect
    const QRect leftPart(itemVR.left(), itemVR.top(), itemVR.width() - shortcutW, itemVR.height());

    int x = leftPart.left() + MC_SPACING;  // beginning

    // --- CHECK/RADIO column ---
    if (hasCheckCol)
    {
        const QRect checkCol(x, leftPart.top(), checkW, leftPart.height());
        R.checkCol = QStyle::visualRect(mi->direction, itemVR, checkCol);
        x += checkW + MC_SPACING; // check/radio & icon/text spacing
    }
    else
        R.checkCol = QRect(); // empty

    // --- ICON column ---
    if (hasIconCol)
    {
        const QRect iconCol(x, leftPart.top(), iconColW, leftPart.height());
        R.iconCol = QStyle::visualRect(mi->direction, itemVR, iconCol);
        x += iconColW + MC_SPACING; // icon & text spacing
    }
    else
        R.iconCol = QRect(); // empty

    // --- TEXT column ---
    const int textW = std::max(0, leftPart.right() - x);
    const QRect textCol(x, leftPart.top(), textW, leftPart.height());
    R.textCol = QStyle::visualRect(mi->direction, itemVR, textCol);


    // --- SHORTCUT column --- (RIGHT PART)
    const QRect shortcutCol(itemVR.right() - shortcutW - MC_SPACING, itemVR.top(), shortcutW, itemVR.height());
    R.shortcutCol = QStyle::visualRect(mi->direction, itemVR, shortcutCol);

    return R;
}

bool WoiStyleMenu::drawMenuItem(const QStyle* base, QStyle::ControlElement ce,
                                const QStyleOption* opt, QPainter* p, const QWidget* w)
{
    if (ce != QStyle::CE_MenuItem) return false;

    const auto* mi = qstyleoption_cast<const QStyleOptionMenuItem*>(opt);
    if (!mi) return false;

    // draw standard menu items by default
    if (mi->menuItemType == QStyleOptionMenuItem::Separator ||
        mi->menuItemType == QStyleOptionMenuItem::Scroller  ||
        mi->menuItemType == QStyleOptionMenuItem::TearOff   ||
        mi->menuItemType == QStyleOptionMenuItem::Margin    ||
        mi->menuItemType == QStyleOptionMenuItem::EmptyArea)
    {
        base->drawControl(ce, opt, p, w);
        return true;
    }

    const bool disabled = !(mi->state & QStyle::State_Enabled);
    const bool isSubmenu = mi->menuItemType == QStyleOptionMenuItem::SubMenu;

    if (mi->state & (QStyle::State_Selected | QStyle::State_MouseOver))
    {
        const QRectF r = QRectF(mi->rect).adjusted(0.5, 0.5, -0.5, -0.5);

        p->save();
        p->setRenderHint(QPainter::Antialiasing, true);
        p->setPen(Qt::NoPen);
        QColor fill = QColor(255, 255, 255, 40);
        if (w)
        {

            const QString txt = mi->text;
            if (txt.contains("quit", Qt::CaseInsensitive) ||
                txt.contains("exit", Qt::CaseInsensitive))
                    fill = QColor(207, 67, 67, 90);
        }
        p->setBrush(fill);
        p->drawRoundedRect(r, CR_RADIUS, CR_RADIUS);
        p->restore();
    }

    const MenuItemRects R = nativeMenuItemRects(base, mi, w, isSubmenu);

    // --- CHECK & RADIO RENDER ---
    if (mi->checkType != QStyleOptionMenuItem::NotCheckable)
    {
        QString iconPath;
        if (mi->checkType == QStyleOptionMenuItem::Exclusive) {
            iconPath = mi->checked
                       ? QString(":/tpIcons/icon_radiobutton_checked.png")
                       : QString(":/tpIcons/icon_radiobutton_unchecked.png");
        } else {
            iconPath = mi->checked
                       ? QString(":/tpIcons/icon_checkbox_checked.png")
                       : QString(":/tpIcons/icon_checkbox_unchecked.png");
        }

        QPixmap pix(iconPath);
        if (!pix.isNull())
        {
            const qreal dpr = w ? w->devicePixelRatioF() : 1.0;
            QPixmap scaled = pix.scaled(R.checkCol.size() * dpr, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            scaled.setDevicePixelRatio(dpr);

            const QSize used = scaled.deviceIndependentSize().toSize();
            const QPoint pos(R.checkCol.center().x() - used.width() / 2, R.checkCol.center().y() - used.height() / 2);

            if (disabled)
                p->setOpacity(0.5);
            else
                p->setOpacity(1.0);
            p->drawPixmap(pos, scaled);
        }
    }

    // --- ICON RENDER ---
    if (!mi->icon.isNull())
    {
        QIcon::Mode mode = disabled
                               ? QIcon::Disabled
                               : mi->state & QStyle::State_Selected ? QIcon::Active : QIcon::Normal;
        QIcon::State state = mi->checked ? QIcon::On : QIcon::Off;

        QPixmap pm = mi->icon.pixmap(M_ICON_SIZE, mode, state);

        base->proxy()->drawItemPixmap(p, R.iconCol, Qt::AlignCenter, pm);
    }

    // --- TEXT ---
    QString label = mi->text;
    QString sc;
    if (int tab = label.indexOf('\t'); tab >= 0)
    {
        sc = label.mid(tab + 1);
        label = label.left(tab);
    }

    QPalette pal = mi->palette;

    // label
    base->proxy()->drawItemText(p, R.textCol, Qt::AlignVCenter | Qt::AlignLeft | Qt::TextShowMnemonic, pal, !disabled,
                                label, QPalette::WindowText);


    // --- RIGHT SIDE: shortcut OR submenu arrow ---
    if (isSubmenu) {
        const QString path = mi->direction == Qt::RightToLeft
            ? Icons::path(Icons::ArrowLeftDT)
            : Icons::path(Icons::ArrowRightDT);

        const qreal dpr = w ? w->devicePixelRatioF() : 1.0;
        QPixmap pm(path);
        if (!pm.isNull()) {
            pm = pm.scaled(QSize(SUBM_ARR_SIZE, SUBM_ARR_SIZE) * dpr, Qt::KeepAspectRatio, Qt::SmoothTransformation);
            pm.setDevicePixelRatio(dpr);

            if (disabled)
                p->setOpacity(0.5);
            else
                p->setOpacity(1.0);
            base->proxy()->drawItemPixmap(p, R.shortcutCol, Qt::AlignVCenter | Qt::AlignRight, pm);
        }
    } else {
        // shortcut
        if (!sc.isEmpty()) {
            base->proxy()->drawItemText(
                p, R.shortcutCol,
                Qt::AlignVCenter | Qt::AlignRight,
                mi->palette, !disabled, sc, QPalette::Text
            );
        }
    }



    return true;
}




bool WoiStyleMenu::drawSeparator(const QStyle* base, QStyle::ControlElement ce, const QStyleOption* opt, QPainter* p,
                                 const QWidget* w)
{
    if (ce != QStyle::CE_MenuItem)
        return false;

    const auto* mi = qstyleoption_cast<const QStyleOptionMenuItem*>(opt);
    if (!mi)
        return false;
    if (mi->menuItemType != QStyleOptionMenuItem::Separator)
        return false;

    const int panelWidth = base->pixelMetric(QStyle::PM_MenuPanelWidth, mi, w);
    const int hmargin = base->pixelMetric(QStyle::PM_MenuHMargin, mi, w);

    const int left = mi->rect.left() + panelWidth + hmargin + SEPARATOR_H_M;
    const int right = mi->rect.right() - panelWidth - hmargin - SEPARATOR_H_M;
    const int y = mi->rect.center().y();

    QColor c = mi->palette.color(QPalette::WindowText);
    c.setAlphaF(0.20);

    p->save();
    p->setRenderHint(QPainter::Antialiasing, false);
    QPen pen(c, 1);
    pen.setCosmetic(true);
    p->setPen(pen);
    p->drawLine(QPoint(left, y), QPoint(right, y));
    p->restore();

    return true;
}


void WoiStyleMenu::updateBlur(const QWidget* w)
{
    QScreen* scr = w->screen();
    if (!scr) scr = qApp->primaryScreen();
    if (!scr) return;

    const QImage blur = WoiStyleUtility::makeBlur(scr, w->geometry(), BLUR_RADIUS);
    blurCache_.insert(w, blur);
}



bool WoiStyleMenuFilter::eventFilter(QObject* watched, QEvent* event)
{
    const QWidget* w = qobject_cast<QWidget*>(watched);
    if (!w)
        return QObject::eventFilter(watched, event);

    QWidget* popup = w->window();
    if (!popup)
        return QObject::eventFilter(watched, event);

    //      filter only for QMenu or QComboBox menu
    if (!qobject_cast<QMenu*>(popup) && !popup->inherits("QComboBoxPrivateContainer"))
        return QObject::eventFilter(watched, event);

    switch (event->type()) {
        case QEvent::Show: {
            if (!WoiStyleMenu::blurCache_.contains(popup))
                WoiStyleMenu::updateBlur(popup);
            break;
        }

        case QEvent::Hide:
        case QEvent::Close:
        case QEvent::DeferredDelete:
        case QEvent::Destroy: {
            WoiStyleMenu::blurCache_.remove(popup);
            break;
        }

        default:
            break;
    }

    return QObject::eventFilter(watched, event);
}
