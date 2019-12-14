#include "graph_widget/graph_layout_progress_widget.h"

#include <QPainter>
#include <QParallelAnimationGroup>
#include <QPropertyAnimation>

graph_layout_progress_widget::graph_layout_progress_widget(QWidget* parent) : QFrame(parent),
    //m_x_animation(new QPropertyAnimation(this, "x", this)),
    m_opacity_animation(new QPropertyAnimation(this, "windowOpacity", this)),
    m_arrow_animation(new QPropertyAnimation(this, "arrow_offset", this)),
    m_animation_group(new QParallelAnimationGroup(this)),
    m_arrow_offset(0),
    m_bar_height(30),
    m_arrow_width(10),
    m_arrow_length(30),
    m_direction(direction::left)
{
    hide();

    // DO TYPE STUFF HERE
//    m_x_animation->setDuration(700);
//    m_x_animation->setEasingCurve(QEasingCurve::OutExpo);
    //m_x_animation->setEasingCurve(QEasingCurve::OutBack);

    m_opacity_animation->setDuration(700);
    m_opacity_animation->setStartValue(0);
    m_opacity_animation->setEndValue(1);
    m_opacity_animation->setEasingCurve(QEasingCurve::Linear);

    m_arrow_animation->setDuration(700);
    m_arrow_animation->setStartValue(0);
    m_arrow_animation->setEndValue(100);
    m_arrow_animation->setLoopCount(-1);
    m_arrow_animation->setEasingCurve(QEasingCurve::Linear);

    //m_animation_group->addAnimation(m_x_animation);
    m_animation_group->addAnimation(m_opacity_animation);
    m_animation_group->addAnimation(m_arrow_animation);

    m_arrow_left.lineTo(QPointF(-(m_arrow_length - m_arrow_width), (m_bar_height / 2)));
    m_arrow_left.lineTo(QPointF(0, m_bar_height));
    m_arrow_left.lineTo(QPointF(-m_arrow_width, m_bar_height));
    m_arrow_left.lineTo(QPointF(-m_arrow_length, (m_bar_height / 2)));
    m_arrow_left.lineTo(QPointF(-m_arrow_width, 0));
    m_arrow_left.closeSubpath();

    m_arrow_right.lineTo(QPointF((m_arrow_length - m_arrow_width), (m_bar_height / 2)));
    m_arrow_right.lineTo(QPointF(0, m_bar_height));
    m_arrow_right.lineTo(QPointF(m_arrow_width, m_bar_height));
    m_arrow_right.lineTo(QPointF(m_arrow_length, (m_bar_height / 2)));
    m_arrow_right.lineTo(QPointF(m_arrow_width, 0));
    m_arrow_right.closeSubpath();
}

void graph_layout_progress_widget::start()
{
    setWindowOpacity(0);
    show();
//    m_x_animation->setStartValue(final_position.x() + 80);
//    m_x_animation->setEndValue(final_position.x());
    m_animation_group->start();
}

void graph_layout_progress_widget::stop()
{
    hide();
    m_animation_group->stop();
}

void graph_layout_progress_widget::set_direction(const direction d)
{
    m_direction = d;
}

int graph_layout_progress_widget::arrow_offset() const
{
    return m_arrow_offset;
}

void graph_layout_progress_widget::set_arrow_offset(const int offset)
{
    m_arrow_offset = offset;
    update();
}

void graph_layout_progress_widget::paintEvent(QPaintEvent* event)
{
    QFrame::paintEvent(event);

    QPainter painter(this);

    QPen pen;
    pen.setCosmetic(true);
    pen.setColor(QColor(85, 255, 0));
    painter.setPen(pen);

    painter.drawRect(QRect(0, 0, width() - 1, height() - 1));

    painter.setClipRect(QRect(0, 0, width(), height()));
    painter.setRenderHint(QPainter::Antialiasing, true);

    //painter.drawText(QRectF(10, 40, 50, 50), QString::number(m_arrow_offset));

    if (m_direction == direction::left)
    {
        qreal value = (40.0 / 100.0) * m_arrow_offset;
        painter.translate(-value, 0);

        for (int x = 0; x < width(); x += 10)
        {
            painter.fillPath(m_arrow_left, QColor(85, 255, 0));
            painter.translate(20, 0);
        }
    }
    else
    {
        qreal value = (40.0 / 100.0) * m_arrow_offset;
        painter.translate(-40, 0);
        painter.translate(value, 0);

        for (int x = 0; x < width(); x += 10)
        {
            painter.fillPath(m_arrow_right, QColor(85, 255, 0));
            painter.translate(20, 0);
        }
    }
}

QSize graph_layout_progress_widget::sizeHint() const
{
    //DEBUG FUNCTION, REPLACE WITH INTERNAL LAYOUT OR SOMETHING LATER
    return QSize(260, 140);
}
