#include "WindowTitleBar.h"
#include <QApplication>
#include <QPushButton>
#include <QLabel>
#include <QHBoxLayout>

WindowTitleBar::WindowTitleBar(QWidget *parent) :
    QWidget(parent)
{
    setFixedHeight(40);
    const QSize buttonSize(40, 40);
    const QSize systemIconSize(16, 16);
    const QSize otherIconSize(24, 24);
    // Title frame
    QLabel *titleIconLabel = new QLabel(this);
    titleIconLabel->setPixmap(QPixmap(":/icons/PE.png").scaled(20,20,Qt::IgnoreAspectRatio, Qt::SmoothTransformation));
    titleIconLabel->setFixedSize(buttonSize);
    QLabel *titleLabel = new QLabel("Photo Editor 1.0", this);
    //titleLabel->setPixmap(QPixmap(":/icons/PE"));

    // System buttons
    QPushButton *trayButton = createButton(QIcon(":/icons/tray.png"), systemIconSize, buttonSize);
    QPushButton *expandButton = createButton(QIcon(":/icons/full_screen.png"), systemIconSize, buttonSize);
    QPushButton *closeButton = createButton(QIcon(":/icons/close.png"), systemIconSize, buttonSize);

    // Addidional buttons
    QPushButton *infoButton = createButton(QIcon(":/icons/info.png"), otherIconSize, buttonSize);
    QPushButton *settingsButton = createButton(QIcon(":/icons/settings.png"), otherIconSize, buttonSize);

    QHBoxLayout *titlelayout = new QHBoxLayout();
    titlelayout->setMargin(10);
    titlelayout->setSpacing(6);

    titlelayout->addWidget(titleIconLabel);
    titlelayout->addWidget(titleLabel);

    QHBoxLayout *headerlayout = new QHBoxLayout(this);
    headerlayout->setMargin(0);
    headerlayout->setSpacing(0);

    headerlayout->addLayout(titlelayout);
    headerlayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    headerlayout->addWidget(infoButton);
    headerlayout->addWidget(settingsButton);
    headerlayout->addItem(new QSpacerItem(16, 20, QSizePolicy::Minimum, QSizePolicy::Minimum));
    headerlayout->addWidget(trayButton);
    headerlayout->addWidget(expandButton);
    headerlayout->addWidget(closeButton);

    setLayout(headerlayout);

    connect(closeButton, &QAbstractButton::released, this,[](){ QApplication::exit();});
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
