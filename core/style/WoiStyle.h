//         Created by Nazuha26 in CLion
//         Date: 28.08.2025
//         Time: 19:34:10

#ifndef WOISTYLE_H
#define WOISTYLE_H

#include <QProxyStyle>



class WoiStyle : public QProxyStyle {
    Q_OBJECT
public:
    explicit WoiStyle(const QString& base);

    QSize sizeFromContents(ContentsType type, const QStyleOption* opt,
                                 const QSize& csz, const QWidget* w) const override;

    int pixelMetric(PixelMetric pm, const QStyleOption* option, const QWidget* w) const override;

    void drawControl(ControlElement element, const QStyleOption* option, QPainter* p,
                     const QWidget* w = nullptr) const override;

    void drawComplexControl(ComplexControl cc, const QStyleOptionComplex* opt, QPainter* p, const QWidget* w) const override;

    void drawPrimitive(PrimitiveElement pe, const QStyleOption* option, QPainter* p,
                       const QWidget* w = nullptr) const override;

    void polish(QPalette& pal) override;
    void polish(QWidget* w) override;
};

#endif // WOISTYLE_H
