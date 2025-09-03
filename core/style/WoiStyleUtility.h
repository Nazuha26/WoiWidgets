//         Created by Nazuha26 in CLion
//         Date: 31.08.2025
//         Time: 20:18:08

#ifndef WOISTYLEUTILITY_H
#define WOISTYLEUTILITY_H

#define FOCUS_BW 2  // focus border width, Using: Button
#define RADIUS 4    // Using: Button

#include <qtypes.h>

class QColor;
class QImage;
class QScreen;
class QRect;

class WoiStyleUtility {
public:
    static QColor mix(const QColor& a, const QColor& b, qreal t);
    static QImage makeBlur(QScreen* scr, const QRect& rect, float sigma);
private:
    static inline qreal clamp01(const qreal v)
    { return v < 0 ? 0 : v > 1 ? 1 : v; }
};



#endif //WOISTYLEUTILITY_H
