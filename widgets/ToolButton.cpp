#include "ToolButton.h"
#include <QPixmap>
#include <QBitmap>
#include <QDebug>

ToolButton::ToolButton(const QIcon &icon, QWidget *parent) :
    QPushButton(icon, QString(), parent),
    m_Icon(icon),
    m_CheckedIcon(icon)
{
    connect(this, &QAbstractButton::toggled, this, [this](bool value)
    {
        value ? setIcon(m_CheckedIcon) : setIcon(m_Icon);
    });
}

void ToolButton::setCheckedIconColor(const QColor &color)
{
    m_ChecketColor = color;
    QImage tmp = m_Icon.pixmap(iconSize()).toImage();

    for(int y = 0; y < tmp.height(); ++y)
    {
      for(int x= 0; x < tmp.width(); ++x)
      {
        QColor color = m_ChecketColor;
        color.setAlpha(tmp.pixelColor(x,y).alpha());
        tmp.setPixelColor(x,y,color);
      }
    }
    QPixmap pixmap = QPixmap::fromImage(tmp);
    m_CheckedIcon = QIcon(pixmap);
}
