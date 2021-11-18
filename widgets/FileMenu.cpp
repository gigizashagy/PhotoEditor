#include "FileMenu.h"
#include <QPainter>
#include <QGraphicsDropShadowEffect>

FileMenu::FileMenu(QWidget *parent) :
    QMenu(parent)
{
    initializeAttributes();
}

FileMenu::FileMenu(const QString &title, QWidget *parent) :
    QMenu(title, parent)
{
    initializeAttributes();
}

QRect FileMenu::rect() const
{
    return QMenu::rect() - QMargins(0, m_ArrowSize.height(), 0, 0);
}

void FileMenu::setArrowSize(const QSize size)
{
    m_ArrowSize = size;
}

void FileMenu::setBorderRadius(uint radius)
{
    m_BorderRadius = radius;
    m_ShadowEffect->setBlurRadius(m_BorderRadius);
}

uint FileMenu::getBorderRadius() const
{
    return m_BorderRadius;
}

void FileMenu::initializeAttributes()
{
    m_ArrowSize = QSize(12, 6);
    m_ArrowRect = QRect(QPoint(14, 1), m_ArrowSize);
    m_BorderRadius = 6;

    setWindowFlags(windowFlags() | Qt::FramelessWindowHint);// | Qt::NoDropShadowWindowHint);
    m_ShadowEffect = new QGraphicsDropShadowEffect(this);
    m_ShadowEffect->setBlurRadius(m_BorderRadius);
    setGraphicsEffect(m_ShadowEffect);
    setAttribute(Qt::WA_TranslucentBackground);
}

void FileMenu::paintEvent(QPaintEvent *event)
{
    QPainter painter(this);

    painter.setRenderHint(QPainter::Antialiasing);
    painter.setBrush(Qt::white);
    painter.setPen(Qt::NoPen);

    QPainterPath arrowShape(QPoint(m_ArrowRect.topLeft().x() + m_ArrowSize.width()/2, m_ArrowRect.top()));
    arrowShape.lineTo(m_ArrowRect.bottomLeft());
    arrowShape.lineTo(m_ArrowRect.bottomRight());

    painter.drawRoundedRect(rect(), m_BorderRadius, m_BorderRadius, Qt::AbsoluteSize);
    painter.drawPath(arrowShape);

    QMenu::paintEvent(event);
}
