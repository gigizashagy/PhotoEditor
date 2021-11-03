#ifndef WINDOWTITLEBAR_H
#define WINDOWTITLEBAR_H

#include <QWidget>

class QPushButton;

class WindowTitleBar : public QWidget
{
    Q_OBJECT
public:
    explicit WindowTitleBar(QWidget *parent = nullptr);

private:
    QPushButton* createButton(const QIcon& icon, QSize iconSize, QSize buttonSize);

};

#endif // WINDOWTITLEBAR_H
