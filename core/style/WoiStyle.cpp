//         Created by Nazuha26 in CLion
//         Date: 28.08.2025
//         Time: 19:34:10

#include "WoiStyle.h"

#include <qabstractitemview.h>
#include <QApplication>
#include <QComboBox>
#include <QFile>
#include <QMenu>
#include <QPainter>
#include <QPainterPath>
#include <QPushButton>

#include "widgets/WoiStyleButtons.h"
#include "widgets/WoiStyleMenu.h"
#include "widgets/WoiStyleTabBar.h"
#include "widgets/WoiTooltip.h"



void applyQss(const QString& qssFilePath)
{
    QFile file(qssFilePath);
    if (file.open(QIODeviceBase::ReadOnly | QIODeviceBase::Text))
    {
        const QString style = QLatin1String(file.readAll());
        qApp->setStyleSheet(style);
    }
}

WoiStyle::WoiStyle(const QString& base) : QProxyStyle(base)
{
    QIcon::setThemeSearchPaths({":/res"});
    QIcon::setThemeName("woi_dark");
    QIcon::setFallbackThemeName("woi_dark");
    applyQss(":/dark/qss/dark.qss");
    WoiTooltip::install();                  // install custom tooltip filter
}



void WoiStyle::polish(QPalette& pal)
{
    QProxyStyle::polish(pal);

    pal.setColor(QPalette::Window, pal.color(QPalette::Window).darker(110));
    pal.setColor(QPalette::Base, pal.color(QPalette::Base).darker(110));
    pal.setColor(QPalette::AlternateBase, pal.color(QPalette::Base).darker(115));
    pal.setColor(QPalette::Text, pal.color(QPalette::Text));
    pal.setColor(QPalette::Button, pal.color(QPalette::Highlight));
    pal.setColor(QPalette::ButtonText, pal.color(QPalette::HighlightedText));
    pal.setColor(QPalette::Highlight, pal.color(QPalette::Highlight));
    pal.setColor(QPalette::HighlightedText, pal.color(QPalette::HighlightedText));
}



void WoiStyle::polish(QWidget* w)
{
    QProxyStyle::polish(w);

    if (QPushButton* btn = qobject_cast<QPushButton*>(w))
        WoiStyleButtons::polishButton(btn);

    if (QMenu* menu = qobject_cast<QMenu*>(w))
        WoiStyleMenu::polishMenu(menu);

    // make QComboBox menu transparent
    QWidget *popup = w->window();
    if (popup && popup->inherits("QComboBoxPrivateContainer")) {
        popup->setAttribute(Qt::WA_TranslucentBackground, true);
        popup->setWindowFlags(popup->windowFlags() | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);

        QPalette pal = popup->palette();
        pal.setBrush(QPalette::Window, Qt::NoBrush);
        pal.setBrush(QPalette::Base,   Qt::NoBrush);
        w->setPalette(pal);
    }
}



QSize WoiStyle::sizeFromContents(const ContentsType type, const QStyleOption* opt,
                                 const QSize& csz, const QWidget* w) const
{
    const QSize s = QProxyStyle::sizeFromContents(type, opt, csz, w);
    if (type == CT_PushButton)
        return WoiStyleButtons::resizeButton(s);

    return s;
}



int WoiStyle::pixelMetric(PixelMetric pm, const QStyleOption* option, const QWidget* w) const
{
    return QProxyStyle::pixelMetric(pm, option, w);
}



void WoiStyle::drawControl(ControlElement element, const QStyleOption* option,
                           QPainter* p, const QWidget* w) const
{
    //      PUSHBUTTON
    if (WoiStyleButtons::drawButton(baseStyle(), element, option, p, w))
        return;

    //      TABBAR
    if (WoiStyleTabBar::drawTabBar(baseStyle(), element, option, p, w))
        return;

    //      SEPARATOR
    if (WoiStyleMenu::drawSeparator(baseStyle(), element, option, p, w))
        return;

    //      MENUITEM
    if (WoiStyleMenu::drawMenuItem(baseStyle(), element, option, p, w))
        return;


    QProxyStyle::drawControl(element, option, p, w);
}



void WoiStyle::drawComplexControl(ComplexControl cc, const QStyleOptionComplex* opt, QPainter* p,
    const QWidget* w) const
{
    QProxyStyle::drawComplexControl(cc, opt, p, w);
}



void WoiStyle::drawPrimitive(PrimitiveElement pe, const QStyleOption* option, QPainter* p, const QWidget* w) const
{
    //  TAB WIDGET
    if (WoiStyleTabBar::drawTabWidget(pe, option, p, w))
        return;

    //  TAB CLOSE BUTTON
    if (WoiStyleTabBar::drawCloseBtn(pe, option, p, w))
        return;

    //  MENU
    if (WoiStyleMenu::drawMenu(pe, option, p, w))
        return;


    QProxyStyle::drawPrimitive(pe, option, p, w);
}
