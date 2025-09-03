//         Created by Nazuha26 in CLion
//         Date: 29.08.2025
//         Time: 16:38:13

#include "WoiStyleTabBar.h"
#include <QLinearGradient>
#include <QPainter>
#include <QStyleOptionTab>
#include "../WoiStyleUtility.h"
#include "../../Icons.h"

#define DARKEN_PERCENT 120



bool WoiStyleTabBar::drawTabBar(const QStyle* base,
                                QStyle::ControlElement element,
                                const QStyleOption* option,
                                QPainter* p,
                                const QWidget* w)
{
    if (element == QStyle::CE_TabBarTabShape) {
        const auto* to = qstyleoption_cast<const QStyleOptionTab*>(option);
        if (!to) { base->drawControl(element, option, p, w); return true; }

        const QPalette& pal = option->palette;
        const bool selected = to->state & QStyle::State_Selected;

        QRect r = to->rect;

        QColor baseFill = pal.window().color().darker(DARKEN_PERCENT);

        const QColor sepLine = baseFill.lighter();
        const QColor accent  = pal.highlight().color();

        p->save();
        p->setRenderHint(QPainter::Antialiasing, false);
        p->fillRect(r, baseFill);

        {
            QPen pen(sepLine, 1);
            pen.setCosmetic(true);
            p->setPen(pen);

            switch (to->shape) {
            case QTabBar::RoundedNorth:
            case QTabBar::TriangularNorth:
                p->translate(0.0, 0.5);
                p->drawLine(r.left(), r.bottom(), r.right(), r.bottom());
                p->translate(0.0, -0.5);
                break;
            case QTabBar::RoundedSouth:
            case QTabBar::TriangularSouth:
                p->translate(0.0, 0.5);
                p->drawLine(r.left(), r.top(), r.right(), r.top());
                p->translate(0.0, -0.5);
                break;
            case QTabBar::RoundedWest:
            case QTabBar::TriangularWest:
                p->translate(0.5, 0.0);
                p->drawLine(r.right(), r.top(), r.right(), r.bottom());
                p->translate(-0.5, 0.0);
                break;
            default: // East
                p->translate(0.5, 0.0);
                p->drawLine(r.left(), r.top(), r.left(), r.bottom());
                p->translate(-0.5, 0.0);
                break;
            }
        }

        if (selected) {
            QPen pen(accent, 4, Qt::SolidLine, Qt::SquareCap, Qt::MiterJoin);
            pen.setCosmetic(true);
            p->setPen(pen);

            switch (to->shape) {
            case QTabBar::RoundedNorth:
            case QTabBar::TriangularNorth:
                p->translate(0.0, 0.5);
                p->drawLine(r.left(), r.bottom(), r.right(), r.bottom());
                break;
            case QTabBar::RoundedSouth:
            case QTabBar::TriangularSouth:
                p->translate(0.0, 0.5);
                p->drawLine(r.left(), r.top(), r.right(), r.top());
                break;
            case QTabBar::RoundedWest:
            case QTabBar::TriangularWest:
                p->translate(0.5, 0.0);
                p->drawLine(r.right(), r.top(), r.right(), r.bottom());
                break;
            default: // East
                p->translate(0.5, 0.0);
                p->drawLine(r.left(), r.top(), r.left(), r.bottom());
                break;
            }
        }

        p->restore();
        return true;
    }

    if (element == QStyle::CE_TabBarTabLabel) {
        const auto* to = qstyleoption_cast<const QStyleOptionTab*>(option);
        if (!to) { base->drawControl(element, option, p, w); return true; }

        QStyleOptionTab o(*to);
        if (!(o.state & QStyle::State_Selected)) {
            QColor t = o.palette.color(QPalette::ButtonText).darker(DARKEN_PERCENT);
            o.palette.setColor(QPalette::ButtonText, t);
            o.palette.setColor(QPalette::WindowText, t);
        }
        base->drawControl(QStyle::CE_TabBarTabLabel, &o, p, w);
        return true;
    }

    return false;
}




bool WoiStyleTabBar::drawTabWidget(const QStyle::PrimitiveElement pe, const QStyleOption* opt, QPainter* p,
                                   const QWidget* w)
{
    const QPalette& pal = opt->palette;
    QColor fillC = pal.window().color().darker(DARKEN_PERCENT);
    if (pe == QStyle::PE_FrameTabWidget)
    {
        p->save();
        p->setBrush(QColor(fillC));
        p->setPen(Qt::NoPen);
        p->drawRect(opt->rect);
        p->restore();
        return true;
    }
    return false;
}



bool WoiStyleTabBar::drawCloseBtn(QStyle::PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w)
{
    if (pe == QStyle::PE_IndicatorTabClose)
    {
        p->save();
        p->setRenderHint(QPainter::Antialiasing, true);

        const QRect r = opt->rect;
        const QIcon::Mode mode = opt->state & QStyle::State_Enabled ? QIcon::Normal : QIcon::Disabled;

        if (opt->state & QStyle::State_MouseOver)
        {
            p->setPen(Qt::NoPen);
            p->setBrush(QColor(255, 255, 255, 0.1 * 255));
            p->drawEllipse(r);
        }

        QIcon closeIcon_(Icons::path(Icons::CloseDT));

        const QRect box = QStyle::alignedRect(Qt::LeftToRight, Qt::AlignCenter, QSize(8, 8), r);
        closeIcon_.paint(p, box, Qt::AlignCenter, mode);

        p->restore();
        return true;
    }

    return false;
}
