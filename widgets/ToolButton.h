#ifndef TOOLBUTTON_H
#define TOOLBUTTON_H

#include <QPushButton>

class ToolButton : public QPushButton
{
    Q_OBJECT
public:
    explicit ToolButton(const QIcon &icon, QWidget *parent = nullptr);
    void setCheckedIconColor(const QColor& color);

private:
    QIcon m_Icon;
    QIcon m_CheckedIcon;
    QColor m_ChecketColor;
};

#endif // TOOLBUTTON_H
