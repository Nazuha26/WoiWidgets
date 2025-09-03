//         Created by Nazuha26 in CLion
//         Date: 29.08.2025
//         Time: 10:11:53

#include "WoiStyleButtons.h"

#include <QPainter>
#include <QPainterPath>
#include <QPushButton>
#include <QStyleOptionButton>
#include "../WoiStyleUtility.h"
#include "../../Icons.h"



static bool isMinimizeBtn(const QWidget* w) {
    if (!w) return false;
    return w->objectName().compare("btnMinimize", Qt::CaseInsensitive) == 0;
}
static bool isMaximizeBtn(const QWidget* w) {
    if (!w) return false;
    return w->objectName().compare("btnMaximize", Qt::CaseInsensitive) == 0;
}
static bool isCloseBtn(const QWidget* w) {
    if (!w) return false;
    return w->objectName().compare("btnClose", Qt::CaseInsensitive) == 0;
}

bool WoiStyleButtons::drawButton(const QStyle* base, QStyle::ControlElement element, const QStyleOption* option,
    QPainter* p, const QWidget* w)
{
    if (element == QStyle::CE_PushButton || element == QStyle::CE_PushButtonBevel) {
        const auto* btnOpt = qstyleoption_cast<const QStyleOptionButton*>(option);
        if (!btnOpt) { base->drawControl(element, option, p, w); return true; }

        // DATA
        const QPalette& pal = btnOpt->palette;
        const QRect rOuter = btnOpt->rect;
        const QRect rInner = rOuter.adjusted(FOCUS_BW, FOCUS_BW, -FOCUS_BW, -FOCUS_BW);

        const bool enabled = btnOpt->state.testFlag(QStyle::State_Enabled);
        const bool focus   = btnOpt->state.testFlag(QStyle::State_HasFocus);
        const bool isDefault = btnOpt->features.testFlag(QStyleOptionButton::DefaultButton);

        // WINDOW CONTROL BUTTONS
        if (isMinimizeBtn(w) || isMaximizeBtn(w) || isCloseBtn(w)) {
            const bool hovered = btnOpt->state.testFlag(QStyle::State_MouseOver);

            QBrush fill;

            if (hovered) {
                if (isCloseBtn(w)) {
                    QLinearGradient closeGradient(0, 0, 0, w->height());
                    closeGradient.setColorAt(0, QColor(0xcf4343));
                    closeGradient.setColorAt(1, QColor(0xd53131));
                    fill = closeGradient;
                } else {
                    QColor hoverColor(0xffffff);
                    hoverColor.setAlphaF(0.15); // 15% alpha
                    fill = hoverColor;
                }
            } else {
                fill = QColor(0,0,0,0); // transparent
            }

            p->save();
            //p->setRenderHint(QPainter::Antialiasing, true);
            p->fillRect(rOuter, fill);
            if (!btnOpt->icon.isNull()) {
                QIcon::Mode mode = !enabled ? QIcon::Disabled : QIcon::Normal;
                QPixmap pix = btnOpt->icon.pixmap(btnOpt->iconSize, mode);
                QSize logicalSize = pix.size() / pix.devicePixelRatio();
                QRect iconRect = QStyle::alignedRect(
                    option->direction,
                    Qt::AlignCenter,
                    logicalSize,
                    rOuter
                );
                p->drawPixmap(iconRect, pix);
            }
            p->restore();
            return true;
        }

        // USUAL BUTTON
        const QColor accent   = pal.color(QPalette::Highlight);
        const QColor onAccent = pal.color(QPalette::HighlightedText);
        const QColor text     = pal.color(QPalette::ButtonText);
        const QColor surface  = pal.color(QPalette::Window);
        const QColor mid      = pal.color(QPalette::Mid);

        QColor fill, border;

        if (isDefault) {
            fill = border = accent;
        } else {
            fill = surface.darker(105);
            border = surface.lighter();
        }

        if (!enabled) {
            fill   = WoiStyleUtility::mix(fill, surface, 0.65);
            border = WoiStyleUtility::mix(border, surface, 0.55);
        }

        // PAINTING
        p->save();
        p->setRenderHint(QPainter::Antialiasing, true);

        constexpr int borderW = 1;
        {
            QPainterPath inner;
            inner.addRoundedRect(rInner, RADIUS, RADIUS);
            p->fillPath(inner, fill);
            p->setPen(QPen(border, borderW));
            p->drawPath(inner);
        }

        // focus
        if (focus && enabled)
        {
            constexpr qreal rInnerRadius = RADIUS;
            constexpr qreal rOuterRadius = RADIUS + FOCUS_BW;

            QPainterPath outer, inner;
            outer.addRoundedRect(rOuter, rOuterRadius, rOuterRadius);
            inner.addRoundedRect(rInner, rInnerRadius, rInnerRadius);

            QPainterPath ring = outer.subtracted(inner);

            p->setPen(Qt::NoPen);
            p->setBrush(pal.color(QPalette::Highlight).lighter(135));
            p->drawPath(ring);
        }
        p->restore();

        // LABEL (TEXT & ICON)
        QStyleOptionButton lbl = *btnOpt;
        if (isDefault)
            lbl.palette.setColor(QPalette::ButtonText, onAccent);
        else if (!enabled)
            lbl.palette.setColor(QPalette::ButtonText, WoiStyleUtility::mix(text, mid, 0.6));
        base->drawControl(QStyle::CE_PushButtonLabel, &lbl, p, w);
        return true;
    }
    return false;
}



QSize WoiStyleButtons::resizeButton(QSize size)
{
    size.setHeight(size.height() + 2 * FOCUS_BW);
    size.setWidth(size.width() + 2 * FOCUS_BW);
    return size;
}



void WoiStyleButtons::polishButton(QPushButton* btn)
{
    if (isMinimizeBtn(btn) || isMaximizeBtn(btn) || isCloseBtn(btn)) {
        btn->setFlat(true);
        btn->setAutoDefault(false);
        btn->setDefault(false);
        btn->setText(QString());
        btn->setFocusPolicy(Qt::NoFocus);
        btn->setIconSize(QSize(12, 12));
    }
    if (isMinimizeBtn(btn))
        btn->setIcon(QIcon(Icons::path(Icons::MinimizeDT)));
    if (isMaximizeBtn(btn))
        btn->setIcon(QIcon(Icons::path(Icons::MaximizeDT)));
    if (isCloseBtn(btn))
        btn->setIcon(QIcon(Icons::path(Icons::CloseDT)));
}