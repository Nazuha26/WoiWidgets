//********************************************************************
//                                                                   *
//         File    : WoiTooltip.h                                    *
//         Author  : Nazuha26                                        *
//                                                                   *
//         Copyright (c) 2025 by Nazuha26                            *
//         All bugs are my own. Handle with care.                    *
//                                                                   *
//********************************************************************

//         Created by Nazuha26 in CLion
//         Date: 31.08.2025
//         Time: 15:08:16

#ifndef WOITOOLTIP_H
#define WOITOOLTIP_H

#include <QPropertyAnimation>
#include <QTimer>
#include <QWidget>



class WoiTooltip final : public QWidget
{
    Q_OBJECT
    Q_PROPERTY(qreal opacity READ windowOpacity WRITE setWindowOpacity)

public:
    static void install(QObject* root = nullptr);      // install filter
    static void remove(QObject* root = nullptr);       // remove filter
    static WoiTooltip* instance();                     // singelton

    // VIEW
    void setCornerRadius(const int r) { radius_ = r; update(); }
    void setPadding(const QMargins& m) { padding_ = m; updateGeometry(); }
    void setBlurRadius(const float r) { blurRadius_ = qMax(0.0f, r); }
    void setBackgroundTint(const QColor& c) { tint_ = c; update(); }
    void setFont(const QFont& f) { QWidget::setFont(f); updateGeometry(); }

protected:
    bool eventFilter(QObject* watched, QEvent* event) override;
    void paintEvent(QPaintEvent* e) override;

private:
    explicit WoiTooltip(QWidget* parent = nullptr);
    ~WoiTooltip() override = default;

    void showTextImpl(const QString& text, const QPoint& globalPos, QWidget* source);
    void hideAnimated(int durationMs = 120);
    void updateBlur();

    QImage imgblur_;   // bg blur image
    QString text_;
    QWidget* source_;
    QPropertyAnimation fade_;
    QTimer autoHide_;
    QPoint anchorPos_;

    // DEFAULT VIEW
    int radius_ = 8;
    QMargins padding_ { 12, 6, 12, 6 };
    float blurRadius_ = 4.0f;
    QColor tint_ { 0, 0, 0, static_cast<int>(0.2 * 255) };
    QColor border_ { 255, 255, 255, 40 };

    static WoiTooltip* self_;
};



#endif //WOITOOLTIP_H
