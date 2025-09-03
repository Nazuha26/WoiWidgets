//         Created by Nazuha26 in CLion
//         Date: 29.08.2025
//         Time: 10:11:53

#ifndef WOISTYLEBUTTONS_H
#define WOISTYLEBUTTONS_H

#include <QStyle>



class QPushButton;



class WoiStyleButtons {
public:
    static bool drawButton(const QStyle* base, QStyle::ControlElement element,
                           const QStyleOption* option, QPainter* p, const QWidget* w);

    static QSize resizeButton(QSize size);

    static void polishButton(QPushButton* btn);
};



#endif //WOISTYLEBUTTONS_H
