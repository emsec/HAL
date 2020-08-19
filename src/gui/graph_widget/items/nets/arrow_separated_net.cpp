#include "gui/graph_widget/items/nets/arrow_separated_net.h"

#include "gui/graph_widget/graph_widget_constants.h"

#include <QPainter>
#include <QPen>
#include <QStyleOptionGraphicsItem>

namespace hal
{
    qreal ArrowSeparatedNet::s_wire_length;
    qreal ArrowSeparatedNet::s_input_arrow_offset;
    qreal ArrowSeparatedNet::s_output_arrow_offset;
    qreal ArrowSeparatedNet::s_arrow_left_x_shift;
    qreal ArrowSeparatedNet::s_arrow_right_x_shift;
    qreal ArrowSeparatedNet::s_arrow_side_length;
    qreal ArrowSeparatedNet::s_arrow_width;
    qreal ArrowSeparatedNet::s_arrow_height;

    qreal ArrowSeparatedNet::s_input_width;
    qreal ArrowSeparatedNet::s_output_width;

    QPainterPath ArrowSeparatedNet::s_arrow;

    void ArrowSeparatedNet::load_settings()
    {
        s_wire_length = 26;

        s_input_arrow_offset = 3;
        s_output_arrow_offset = 3;

        s_arrow_left_x_shift = 0;
        s_arrow_right_x_shift = 3;
        s_arrow_side_length = 12;

        s_arrow_height = 6;
        s_arrow_width = s_arrow_left_x_shift + s_arrow_side_length + s_arrow_right_x_shift;

        s_input_width = s_wire_length + s_input_arrow_offset + s_arrow_width + s_shape_width;
        s_output_width = s_wire_length + s_output_arrow_offset + s_arrow_width + s_shape_width;

        QPointF point(s_arrow_left_x_shift, -s_arrow_height / 2);

        #if (QT_VERSION >= QT_VERSION_CHECK(5, 13, 0))
            // only available in Qt >= 5.13.0
            s_arrow.clear();
        #else
            // low-performance fallback for older Qt
            s_arrow = QPainterPath();
        #endif
        s_arrow.lineTo(point);
        point.setX(point.x() + s_arrow_side_length);
        s_arrow.lineTo(point);
        point.setX(point.x() + s_arrow_right_x_shift);
        point.setY(0);
        s_arrow.lineTo(point);
        point.setX(point.x() - s_arrow_right_x_shift);
        point.setY(s_arrow_height / 2);
        s_arrow.lineTo(point);
        point.setX(point.x() - s_arrow_side_length);
        s_arrow.lineTo(point);
        s_arrow.closeSubpath();
    }

    ArrowSeparatedNet::ArrowSeparatedNet(Net* n) : SeparatedGraphicsNet(n)
    {
    }

    void ArrowSeparatedNet::paint(QPainter* painter, const QStyleOptionGraphicsItem* option, QWidget* widget)
    {
        Q_UNUSED(widget);

        if (s_lod < graph_widget_constants::separated_net_min_lod)
            return;

        QColor color = penColor(option->state);
        color.setAlphaF(s_alpha);

        s_pen.setColor(color);
        painter->setPen(s_pen);

        if (m_fill_icon)
        {
            s_brush.setColor(color);
            s_brush.setStyle(m_brush_style);
            painter->setBrush(s_brush);
        }

        const Qt::PenStyle original_pen_style = s_pen.style();

        for (const QPointF& position : m_input_positions)
        {
            QPointF to(position.x() - s_wire_length, position.y());
            painter->drawLine(position, to);
            painter->save();
            painter->setRenderHint(QPainter::Antialiasing, true);
            s_pen.setStyle(Qt::PenStyle::SolidLine);
            painter->setPen(s_pen);
            painter->translate(QPointF(position.x() - s_wire_length - s_input_arrow_offset - s_arrow_width, position.y()));
            painter->drawPath(s_arrow);
            s_pen.setStyle(original_pen_style);
            painter->restore();
        }

        for (const QPointF& position : m_output_positions)
        {
            QPointF to(position.x() + s_wire_length, position.y());
            painter->drawLine(position, to);
            painter->save();
            painter->setRenderHint(QPainter::Antialiasing, true);
            s_pen.setStyle(Qt::PenStyle::SolidLine);
            painter->setPen(s_pen);
            painter->translate(QPointF(position.x() + s_wire_length + s_output_arrow_offset, position.y()));
            painter->drawPath(s_arrow);
            s_pen.setStyle(original_pen_style);
            painter->restore();
        }

        s_brush.setStyle(Qt::NoBrush);
        painter->setBrush(s_brush);

    #ifdef HAL_DEBUG_GUI_GRAPH_WIDGET
        bool original_cosmetic = s_pen.isCosmetic();
        s_pen.setCosmetic(true);
        s_pen.setColor(Qt::green);
        painter->setPen(s_pen);
        painter->drawPath(m_shape);
        s_pen.setCosmetic(original_cosmetic);
    #endif
    }

    void ArrowSeparatedNet::add_input(const QPointF& scene_position)
    {
        QPointF mapped_position = mapFromScene(scene_position);
        m_input_positions.append(mapped_position);

        const qreal half_of_shape_width = s_shape_width / 2;

        QPointF point(mapped_position.x() - s_wire_length - half_of_shape_width, mapped_position.y() - half_of_shape_width);

        m_shape.moveTo(point);
        point.setX(point.x() + s_wire_length + s_shape_width);
        m_shape.lineTo(point);
        point.setY(point.y() + s_shape_width);
        m_shape.lineTo(point);
        point.setX(point.x() - s_wire_length - s_shape_width);
        m_shape.lineTo(point);
        m_shape.closeSubpath();

        point.setX(mapped_position.x() - s_wire_length - s_input_arrow_offset - s_arrow_width - half_of_shape_width);
        point.setY(mapped_position.y() - s_arrow_height / 2 - half_of_shape_width);

        m_shape.moveTo(point);
        point.setX(point.x() + s_arrow_width + s_shape_width);
        m_shape.lineTo(point);
        point.setY(point.y() + s_arrow_height + s_shape_width);
        m_shape.lineTo(point);
        point.setX(point.x() - s_arrow_width - s_shape_width);
        m_shape.lineTo(point);
        m_shape.closeSubpath();
    }

    void ArrowSeparatedNet::add_output(const QPointF& scene_position)
    {
        QPointF mapped_position = mapFromScene(scene_position);
        m_output_positions.append(mapped_position);

        const qreal half_of_shape_width = s_shape_width / 2;

        QPointF point(mapped_position.x() - half_of_shape_width, mapped_position.y() - half_of_shape_width);

        m_shape.moveTo(point);
        point.setX(point.x() + s_wire_length + s_shape_width);
        m_shape.lineTo(point);
        point.setY(point.y() + s_shape_width);
        m_shape.lineTo(point);
        point.setX(point.x() - s_wire_length - s_shape_width);
        m_shape.lineTo(point);
        m_shape.closeSubpath();

        point.setX(mapped_position.x() + s_wire_length + s_output_arrow_offset - half_of_shape_width);
        point.setY(mapped_position.y() - s_arrow_height / 2 - half_of_shape_width);

        m_shape.moveTo(point);
        point.setX(point.x() + s_arrow_width + s_shape_width);
        m_shape.lineTo(point);
        point.setY(point.y() + s_arrow_height + s_shape_width);
        m_shape.lineTo(point);
        point.setX(point.x() - s_arrow_width - s_shape_width);
        m_shape.lineTo(point);
        m_shape.closeSubpath();
    }

    qreal ArrowSeparatedNet::input_width() const
    {
        return s_input_width;
    }

    qreal ArrowSeparatedNet::output_width() const
    {
        return s_output_width;
    }
}
