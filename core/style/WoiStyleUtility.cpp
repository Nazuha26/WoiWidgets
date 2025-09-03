//         Created by Nazuha26 in CLion
//         Date: 31.08.2025
//         Time: 20:18:08

#include "WoiStyleUtility.h"

#include <QScreen>
#include <QPixmap>

#define IIR_GAUSS_BLUR_IMPLEMENTATION
#include "../thirdparty/iir_gauss_blur.h"

QColor WoiStyleUtility::mix(const QColor& a, const QColor& b, qreal t)
{
    t = clamp01(t);
    return QColor::fromRgbF(a.redF() * (1.0 - t) + b.redF() * t, a.greenF() * (1.0 - t) + b.greenF() * t,
                            a.blueF() * (1.0 - t) + b.blueF() * t, a.alphaF() * (1.0 - t) + b.alphaF() * t);
}

QImage WoiStyleUtility::makeBlur(QScreen* scr, const QRect& rect, float sigma)
{
    if (!scr) return {};

    const QPixmap grab = scr->grabWindow(0, rect.x(), rect.y(), rect.width(), rect.height());
    QImage img = grab.toImage().convertToFormat(QImage::Format_ARGB32);

    // Gaussian blur
    const int w = img.width();
    const int h = img.height();
    constexpr int comp = 4;     // RGBA
    iir_gauss_blur(w, h, comp, img.bits(), sigma);

    return img;
}