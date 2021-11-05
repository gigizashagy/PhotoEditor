#include "WindowTitleBar.h"
#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>
#include <QMouseEvent>
#include <QtDebug>

#ifdef Q_OS_WIN
#pragma comment (lib, "user32.lib")
#include <qt_windows.h>
#endif

WindowTitleBar::WindowTitleBar(QWidget *widget) :
    QWidget(widget)
{
    setFixedHeight(40);
    const QSize systemButtonSize(40, 40);
    const QSize systemIconSize(16, 16);
    const QSize otherIconSize(24, 24);

    m_IconsState = {{IconNormal, QIcon(":/icons/down_screen.png")},
                    {IconMaximized, QIcon(":/icons/full_screen.png")}};

    // Title frame
    m_IconLabel = new QLabel(this);
    m_IconLabel->setFixedSize(20, 20);
    m_IconLabel->setScaledContents(true);
    m_IconLabel->setAlignment(Qt::AlignCenter);
    m_IconLabel->setPixmap(windowIcon().pixmap(m_IconLabel->size()));

    m_TitleLabel = new QLabel(this);
    m_TitleLabel->setAlignment(Qt::AlignLeft | Qt::AlignVCenter);
    m_TitleLabel->setText(window()->windowTitle());

    // System buttons
    m_MinimizeButton = createButton(QIcon(":/icons/tray.png"), systemIconSize, systemButtonSize);
    m_MinimizeButton->setToolTip(tr("Minimize"));

    m_MaximizeButton = createButton(m_IconsState[IconMaximized], systemIconSize, systemButtonSize);
    m_MaximizeButton->setToolTip(tr("Maximize"));

    m_CloseButton = createButton(QIcon(":/icons/close.png"), systemIconSize, systemButtonSize);
    m_CloseButton->setObjectName("CloseButton");
    m_CloseButton->setToolTip(tr("Close"));

    // Addidional buttons
    QPushButton *infoButton = createButton(QIcon(":/icons/info.png"), otherIconSize, systemButtonSize);
    QPushButton *settingsButton = createButton(QIcon(":/icons/settings.png"), otherIconSize, systemButtonSize);

    QHBoxLayout *titlelayout = new QHBoxLayout();
    titlelayout->setMargin(10);
    titlelayout->setSpacing(6);

    titlelayout->addWidget(m_IconLabel);
    titlelayout->addWidget(m_TitleLabel);

    QHBoxLayout *headerlayout = new QHBoxLayout(this);
    headerlayout->setMargin(0);
    headerlayout->setSpacing(0);

    headerlayout->addLayout(titlelayout);
    headerlayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    headerlayout->addWidget(infoButton);
    headerlayout->addWidget(settingsButton);
    headerlayout->addItem(new QSpacerItem(16, 20, QSizePolicy::Minimum, QSizePolicy::Minimum));
    headerlayout->addWidget(m_MinimizeButton);
    headerlayout->addWidget(m_MaximizeButton);
    headerlayout->addWidget(m_CloseButton);

    setLayout(headerlayout);

    connect(m_MinimizeButton, &QAbstractButton::clicked, this,[this](){ window()->showMinimized(); });
    connect(m_MaximizeButton, &QAbstractButton::clicked, this,[this](){ window()->isMaximized() ? window()->showNormal() : window()->showMaximized(); });
    connect(m_CloseButton, &QAbstractButton::clicked, this,[this](){ window()->close(); });
}

QPushButton *WindowTitleBar::createButton(const QIcon &icon, QSize iconSize, QSize buttonSize)
{
    QPushButton *button = new QPushButton(this);
    button->setIcon(icon);
    button->setIconSize(iconSize);
    button->setFlat(true);
    button->setFixedSize(buttonSize);
    return button;
}

void WindowTitleBar::updateMaximize()
{
    QWidget *window = this->window();
    if (window->isWindow())
    {
        if (window->isMaximized())
        {
            m_MaximizeButton->setIcon(m_IconsState[IconNormal]);
            m_MaximizeButton->setToolTip(tr("Restore Down"));
        }
        else
        {
            m_MaximizeButton->setIcon(m_IconsState[IconMaximized]);
            m_MaximizeButton->setToolTip(tr("Maximize"));
        }
    }
}

void WindowTitleBar::mousePressEvent(QMouseEvent *event)
{
#ifdef Q_OS_WIN
    if (ReleaseCapture())
    {
        QWidget* window = this->window();
        if (window->isWindow())
        {
            SendMessage (HWND(window-> winId()), WM_SYSCOMMAND, SC_MOVE + HTCAPTION, 0);
        }
    }
    event->ignore();
#endif
}

void WindowTitleBar::mouseDoubleClickEvent(QMouseEvent *event)
{
    if (event->buttons() & Qt::LeftButton)
    {
        m_MaximizeButton->click();
        event->accept();
    }
}

bool WindowTitleBar::eventFilter(QObject *watched, QEvent *event)
{
    Q_UNUSED(event);
    switch (event->type())
    {
    case QEvent::WindowTitleChange:
    {
        QWidget *widget = qobject_cast<QWidget*>(watched);
        if (widget)
        {
            m_TitleLabel->setText(widget->windowTitle());
            return true;
        }
    }
    case QEvent::WindowIconChange:
    {
        QWidget *widget = qobject_cast<QWidget*>(watched);
        if (widget)
        {
            QIcon icon = widget->windowIcon();
            m_IconLabel->setPixmap(icon.pixmap(m_IconLabel->size()));
            return true;
        }
    }
    case QEvent::WindowStateChange:
    case QEvent::Resize:
        updateMaximize();
        return true;
    }
    return QWidget::eventFilter(watched, event);
}
