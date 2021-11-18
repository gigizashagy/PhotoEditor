#ifndef FILEMENU_H
#define FILEMENU_H

#include <QMenu>

class QGraphicsDropShadowEffect;

class FileMenu : public QMenu
{
    Q_OBJECT
public:
    explicit FileMenu(QWidget *parent = nullptr);
    explicit FileMenu(const QString &title, QWidget *parent = nullptr);
    ~FileMenu() = default;

    QRect rect() const;
    void setArrowSize(QSize size);

    void setBorderRadius(uint radius);
    uint getBorderRadius() const;

private:
    void initializeAttributes();

    // QWidget interface
protected:
    void paintEvent(QPaintEvent *event) override;

private:
    QSize m_ArrowSize;
    QRect m_ArrowRect;
    uint m_BorderRadius;

    QGraphicsDropShadowEffect* m_ShadowEffect;
};

#endif // FILEMENU_H
