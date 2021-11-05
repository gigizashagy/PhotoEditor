#ifndef WINDOWTITLEBAR_H
#define WINDOWTITLEBAR_H

#include <QWidget>

class QPushButton;
class QLabel;

class WindowTitleBar : public QWidget
{
    Q_OBJECT
private:
    enum WindowIconState
    {
        IconNormal,
        IconMaximized
    };

public:
    explicit WindowTitleBar(QWidget *widget = nullptr);

private:
    QPushButton* createButton(const QIcon& icon, QSize iconSize, QSize buttonSize);
    void updateMaximize();
    // QWidget interface
protected:
    void mousePressEvent(QMouseEvent *event) override;
    void mouseDoubleClickEvent(QMouseEvent *event) override;
    bool eventFilter(QObject *watched, QEvent *event) override;

private:

    QPushButton *m_MinimizeButton;
    QPushButton *m_MaximizeButton;
    QPushButton *m_CloseButton;
    QLabel *m_IconLabel;
    QLabel *m_TitleLabel;
    QHash<WindowIconState, QIcon> m_IconsState;
};

#endif // WINDOWTITLEBAR_H
