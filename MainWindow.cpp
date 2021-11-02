#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QLayout>
#include <QColorDialog>
#include <QFileDialog>
#include <QMenu>

#include "widgets/ColorPickButton.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    statusBar()->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
    m_ColorButton = new ColorPickButton(ui->centralwidget);
    m_ColorButton->setContentsMargins(2,2,2,2);
    m_ColorButton->setFixedSize(72, 40);

    QMenu* fileMenu = new QMenu(ui->centralwidget);
    fileMenu->addAction(tr("Open file"), this, &MainWindow::openFileDialog, QKeySequence::Open);
    fileMenu->addSeparator();
    fileMenu->addAction(tr("Save"), [](){}, QKeySequence::Save);
    auto actionSaveAs = fileMenu->addAction(tr("Save as..."), [](){}, QKeySequence(Qt::SHIFT + Qt::CTRL + Qt::Key_S));
    actionSaveAs->setEnabled(false);
    fileMenu->addSeparator();
    fileMenu->addAction(tr("Print"), [](){}, QKeySequence::Print);

    ui->FileButton->setMenu(fileMenu);
    ui->OutlineWidget->layout()->addWidget(m_ColorButton);
    imageWidget = new QLabel();
   // ui->scrollArea->setWidget(imageWidget);
    connect(m_ColorButton, &QAbstractButton::pressed, this, &MainWindow::showColorDialog);
}

MainWindow::~MainWindow()
{
    delete ui;
    delete imageWidget;
    delete m_ColorButton;
}

void MainWindow::showColorDialog()
{
    QColor color = QColorDialog::getColor(Qt::white, this);
    m_ColorButton->setColor(color);
}

void MainWindow::openFileDialog()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open file"), QString(), "*.png, *.jpg");
    if (!filePath.isEmpty())
    {
        imageWidget->setPixmap(QPixmap(filePath).scaled(834, 468));
    }
}
