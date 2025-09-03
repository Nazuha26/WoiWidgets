//         Created by Nazuha26 in CLion
//         Date: 30.08.2025
//         Time: 18:06:30

#ifndef WOISTYLEMENU_H
#define WOISTYLEMENU_H

#include <QHash>
#include <QStyle>



class QMenu;



class WoiStyleMenu {
public:
    static void polishMenu(QMenu* m);
    static bool drawMenu(QStyle::PrimitiveElement pe, const QStyleOption* opt, QPainter* p, const QWidget* w);
    static bool drawMenuItem(const QStyle* base, QStyle::ControlElement ce, const QStyleOption* opt, QPainter* p,
                             const QWidget* w);
    static bool drawSeparator(const QStyle* base, QStyle::ControlElement ce, const QStyleOption* opt, QPainter* p,
                              const QWidget* w);

private:
    friend class WoiStyleMenuFilter;
    static void updateBlur(const QWidget* w);

    // cache for a specific menu widget
    static inline QHash<const QWidget*, QImage> blurCache_ { };
};



class WoiStyleMenuFilter : public QObject {
    Q_OBJECT

public:
    explicit WoiStyleMenuFilter(QObject* parent = nullptr) : QObject(parent) { }

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
};



#endif //WOISTYLEMENU_H