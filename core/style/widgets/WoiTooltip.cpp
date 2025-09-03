//         Created by Nazuha26 in CLion
//         Date: 31.08.2025
//         Time: 15:08:16

#include "WoiTooltip.h"

#include <QApplication>
#include <QPainter>
#include <QPainterPath>
#include <QPropertyAnimation>
#include <QStyleOption>
#include <QTextLayout>
#include <QTimer>

#include "../WoiStyleUtility.h"

#define MAX_WIDTH 420
#define OFFSET 12, 12

WoiTooltip* WoiTooltip::self_ = nullptr;



WoiTooltip::WoiTooltip(QWidget* parent) : QWidget(parent),
                                          fade_(this, "opacity")
{
    setAttribute(Qt::WA_TranslucentBackground, true);
    setWindowFlags(Qt::ToolTip | Qt::FramelessWindowHint | Qt::NoDropShadowWindowHint);
    setMouseTracking(false);
    setWindowOpacity(0.0);

    fade_.setEasingCurve(QEasingCurve::OutCubic);
    connect(&autoHide_, &QTimer::timeout, this, [this] { hideAnimated(); });
}



WoiTooltip* WoiTooltip::instance()
{
    if (!self_) self_ = new WoiTooltip();
    return self_;
}



void WoiTooltip::install(QObject* root)
{
    QObject* target = root ? root : qApp;
    target->installEventFilter(instance());
}



void WoiTooltip::remove(QObject* root)
{
    QObject* target = root ? root : qApp;
    target->removeEventFilter(instance());
}



bool WoiTooltip::eventFilter(QObject* watched, QEvent* event)
{
    switch (event->type())
    {
        case QEvent::ToolTip: {
            const QHelpEvent* he  = dynamic_cast<QHelpEvent*>(event);
            QWidget* src = qobject_cast<QWidget*>(watched);
            const QString tip = src ? src->toolTip() : QString();
            if (tip.isEmpty())
                { hideAnimated(); return true; }

            // do not show again if it has already shown
            // and continue the current show
            if (isVisible() && src == source_ && tip == text_) {
                const int dur = toolTipDuration();
                autoHide_.start(dur > 0 ? dur : 4000);
                return true;
            }

            showTextImpl(tip, he->globalPos() + QPoint(OFFSET), src);
            anchorPos_ = he->globalPos();
            return true;
        }
        case QEvent::Leave:
        case QEvent::WindowDeactivate:
        case QEvent::MouseButtonPress:
        case QEvent::FocusOut: {
            if (isVisible()) hideAnimated();
            break;
        }
        default: break;
    }
    return QObject::eventFilter(watched, event);
}



static QRect availRectAt(const QPoint& gp, const QWidget* src) {
    if (const QScreen* s = QGuiApplication::screenAt(gp))
        return s->availableGeometry();
    if (src && src->screen())
        return src->screen()->availableGeometry();
    return QGuiApplication::primaryScreen()->availableGeometry();
}
void WoiTooltip::showTextImpl(const QString& text, const QPoint& globalPos, QWidget* source)
{
    text_ = text;
    source_ = source;

    const QFontMetrics fm(font());

    const QRect textRect = fm.boundingRect(
        QRect(0, 0, MAX_WIDTH, INT_MAX),
        Qt::TextWordWrap | Qt::AlignLeft,
        text
    );

    const QSize content = textRect.size();
    const QSize container = content + QSize(padding_.left() + padding_.right(),
                                     padding_.top()  + padding_.bottom());


    const QRect avail = availRectAt(globalPos, source);

    QPoint pos = globalPos;

    // clamp toolip position so that it does not go beyond the edges
    const int minX = avail.left();
    const int maxX = avail.right() - container.width();
    pos.setX(std::clamp(pos.x(), minX, maxX));

    const int minY = avail.top();
    const int maxY = avail.bottom() - container.height();
    pos.setY(std::clamp(pos.y(), minY, maxY));

    move(pos);
    resize(container);

    updateBlur();

    fade_.stop();
    fade_.setDuration(140);
    fade_.setStartValue(windowOpacity());
    fade_.setEndValue(1.0);
    show();
    raise();
    fade_.start();

    int dur = toolTipDuration();
    if (dur <= 0) dur = 4000;
    autoHide_.start(dur);
}



void WoiTooltip::hideAnimated(const int durationMs)
{
    if (!isVisible()) return;

    autoHide_.stop();

    fade_.stop();
    fade_.setDuration(durationMs);
    fade_.setStartValue(windowOpacity());
    fade_.setEndValue(0.0);
    connect(&fade_, &QPropertyAnimation::finished, this, [this]
    {
        if (qFuzzyIsNull(windowOpacity()))
            hide();
        imgblur_ = QImage();    // clear memory
    });
    fade_.start();
}



void WoiTooltip::updateBlur()
{
    QScreen* scr = screen();
    if (!scr) scr = qApp->primaryScreen();
    if (!scr) return;

    imgblur_ = WoiStyleUtility::makeBlur(scr, geometry(), blurRadius_);
}




void WoiTooltip::paintEvent(QPaintEvent*)
{
    QPainter p(this);
    p.setRenderHint(QPainter::Antialiasing, true);

    const QRectF r = QRectF(rect()).adjusted(0.5, 0.5, -0.5, -0.5);    // for border

    QPainterPath clip;
    clip.addRoundedRect(r, radius_, radius_);
    p.setClipPath(clip);

    if (!imgblur_.isNull())
        p.drawImage(QPoint(0, 0), imgblur_);

    p.fillPath(clip, tint_);

    p.setClipping(false);
    p.setPen(QPen(border_, 1));
    p.setBrush(Qt::NoBrush);
    p.drawRoundedRect(r, radius_, radius_);

    const QPalette pal = qApp->palette();
    p.setPen(pal.toolTipText().color());
    const QRect textRect = rect().marginsRemoved(padding_);
    p.drawText(textRect, Qt::AlignLeft | Qt::AlignVCenter | Qt::TextWordWrap, text_);
}
