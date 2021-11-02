#ifndef COLOPICKBUTTON_H
#define COLOPICKBUTTON_H

#include <QPushButton>

class ColorPickButton : public QPushButton
{
    Q_OBJECT
    Q_PROPERTY(QColor color READ color WRITE setColor)

public:
    ColorPickButton(QWidget *parent = nullptr);

    QColor color() const;
    Q_SLOT void setColor(QColor color);
    void resetColor();
    QSize minimumSizeHint() const override;

protected:
    void paintEvent(QPaintEvent *event) override;
    void resizeEvent(QResizeEvent *event) override;

private:
    void drawCircle(QPainter* painter);
    void drawArrow(QPainter* painter);

private:
    const QSize m_MinimumSize;
    QColor m_Color;
    bool m_IsEmptyColor;
};

#endif // COLOPICKBUTTON_H
