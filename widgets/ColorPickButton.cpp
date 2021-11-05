#include "ColorPickButton.h"
#include <QPainter>

namespace  {
const QColor NO_COLOR("#FF6969");
const QColor ARROW_COLOR("#DADEE3");
const float PEN_SIZE_COEFFITIENT = 0.107f;
const QPoint ARROW_SIZE(12, 6);
const float PEN_ARROW_WITH = 2.f;

}
ColorPickButton::ColorPickButton(QWidget *parent) :
    QPushButton(parent),
    m_MinimumSize(72, 40),
    m_IsEmptyColor(true)
{}

QColor ColorPickButton::color() const
{
    return m_Color;
}

void ColorPickButton::setColor(QColor color)
{
    m_Color = color;
    m_IsEmptyColor = false;
}

void ColorPickButton::resetColor()
{
    m_IsEmptyColor = false;
}

QSize ColorPickButton::minimumSizeHint() const
{
    return m_MinimumSize;
}

void ColorPickButton::paintEvent(QPaintEvent *event)
{
    QPushButton::paintEvent(event);
    QPainter painter(this);
    drawCircle(&painter);
    drawArrow(&painter);
}

void ColorPickButton::resizeEvent(QResizeEvent *event)
{
    QPushButton::resizeEvent(event);

}

void ColorPickButton::drawCircle(QPainter *painter)
{
    const auto content = contentsRect() - contentsMargins();
    const auto diameter = qMin(content.width(), content.height());
    const float penWith = diameter * PEN_SIZE_COEFFITIENT;
    const auto halfPen = penWith / 2;
    const QRectF circleRect = QRect(content.topLeft(), QSize(diameter, diameter)) - QMarginsF(halfPen, halfPen, halfPen, halfPen);
    painter->setRenderHint(QPainter::Antialiasing);

    if (m_IsEmptyColor)
    {
        QPen pen(NO_COLOR, penWith, Qt::SolidLine, Qt::RoundCap);
        painter->setPen(pen);

        painter->drawEllipse(circleRect);
        painter->translate(circleRect.center());
        painter->rotate(45.f);
        painter->drawLine(QPoint(0, -diameter / 2 + halfPen), QPoint(0, diameter / 2 - halfPen));
        painter->resetTransform();
    }
    else
    {
        painter->setPen(Qt::NoPen);
        painter->setBrush(QBrush(m_Color, Qt::SolidPattern));
        painter->drawEllipse(circleRect);
    }
}

void ColorPickButton::drawArrow(QPainter *painter)
{
    const QPoint arrowCenter = contentsRect().center() + QPoint(contentsRect().width() / 4, 0);
    const QRect arrowRect(arrowCenter - ARROW_SIZE / 2, arrowCenter + ARROW_SIZE / 2);

    painter->setPen(QPen(ARROW_COLOR, PEN_ARROW_WITH, Qt::SolidLine, Qt::RoundCap));
    painter->setBrush(Qt::NoBrush);
    painter->drawLine(arrowRect.topLeft(), arrowRect.center() + QPoint(0, arrowRect.height()/2));
    painter->drawLine(arrowRect.topRight(), arrowRect.center() + QPoint(0, arrowRect.height()/2));
}
