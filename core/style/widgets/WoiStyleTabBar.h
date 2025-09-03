//         Created by Nazuha26 in CLion
//         Date: 29.08.2025
//         Time: 16:38:13

#ifndef WOISTYLETABBAR_H
#define WOISTYLETABBAR_H



#include <QStyle>



class QStyleOptionTab;



class WoiStyleTabBar {
public:
    static bool drawTabBar(const QStyle* base, QStyle::ControlElement element,
                            const QStyleOption* option, QPainter* p, const QWidget* w);

    static bool drawTabWidget(QStyle::PrimitiveElement pe,
                             const QStyleOption* opt,
                             QPainter* p,
                             const QWidget* w);

    static bool drawCloseBtn(QStyle::PrimitiveElement pe,
                             const QStyleOption* opt,
                             QPainter* p,
                             const QWidget* w);

private:
    static QPainterPath tabPath(const QRect& rIn, const QStyleOptionTab* to);
};



#endif //WOISTYLETABBAR_H
