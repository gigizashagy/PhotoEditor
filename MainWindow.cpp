#include "MainWindow.h"

#include <QLayout>
#include <QColorDialog>
#include <QFileDialog>
#include <QMenu>
#include <QLabel>
#include <QLineEdit>
#include <QSlider>
#include <QButtonGroup>
#include <QScrollArea>
#include <QStatusBar>
#include <QDebug>

#include "widgets/ToolButton.h"
#include "widgets/ColorPickButton.h"
#include "widgets/WindowTitleBar.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent)
{

    setWindowFlags (Qt::FramelessWindowHint | windowFlags());
    setWindowTitle("Photo Editor 1.0");
    setWindowIcon(QIcon(":/icons/PE.png"));
    setCentralWidget(new QWidget(this));
    centralWidget()->setObjectName(QString::fromUtf8("CentralWidget"));
    this->setContentsMargins(1, 1, 1, 1);
    QVBoxLayout *centralVLayout = new QVBoxLayout(centralWidget());
    centralVLayout->setSpacing(0);
    centralVLayout->setMargin(0);
    centralWidget()->setLayout(centralVLayout);

    // Title
    m_TitleBar = new WindowTitleBar(this);
    installEventFilter(m_TitleBar);
    centralVLayout->addWidget(m_TitleBar);
    centralVLayout->addWidget(createLine(HorizontalLine, centralWidget()));
    centralVLayout->addWidget(createHeaderBar());
    centralVLayout->addWidget(createLine(HorizontalLine, centralWidget()));

    //CentralArea
    QHBoxLayout *bodyHLayout = new QHBoxLayout();
    bodyHLayout->setSpacing(0);
    bodyHLayout->addWidget(createEditArea());
    bodyHLayout->addWidget(createToolPanel());
    centralVLayout->addLayout(bodyHLayout);
    centralVLayout->addWidget(createLine(HorizontalLine, centralWidget()));

    setStatusBar(new QStatusBar(this));
    statusBar()->setObjectName(QString::fromUtf8("StatusBar"));
    statusBar()->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
}

MainWindow::~MainWindow()
{
}

void MainWindow::showColorDialog()
{
    QColor color = QColorDialog::getColor(color, nullptr);
    auto colorButton = findChild<ColorPickButton*>("ColorPickButton");
    if (color.isValid())
    {
        colorButton->setColor(color);
    }
    colorButton->resetColor();
}

void MainWindow::openFileDialog()
{
    QString filePath = QFileDialog::getOpenFileName(this, tr("Open file"), QString(), "*.png, *.jpg");
    if (!filePath.isEmpty())
    {
        m_EditWidget->setPixmap(QPixmap(filePath).scaled(834, 468));
    }
}

QWidget *MainWindow::createHeaderBar()
{
    QWidget* headerWidget = new QWidget(centralWidget());
    headerWidget->setObjectName(QString::fromUtf8("HeaderWidget"));
    headerWidget->setFixedHeight(64);

    m_FileButton = new QPushButton(tr("File"), headerWidget);
    m_FileButton->setObjectName(QString::fromUtf8("FileButton"));
    QSizePolicy sizePolicy(QSizePolicy::Fixed, QSizePolicy::Fixed);
    sizePolicy.setHorizontalStretch(0);
    sizePolicy.setVerticalStretch(0);
    sizePolicy.setHeightForWidth(m_FileButton->sizePolicy().hasHeightForWidth());
    m_FileButton->setSizePolicy(sizePolicy);
    m_FileButton->setFixedSize(QSize(70, 40));
    m_FileButton->setFlat(true);

    QMenu* fileMenu = new QMenu(headerWidget);
    fileMenu->addAction(tr("Open file"), this, &MainWindow::openFileDialog, QKeySequence::Open);
    fileMenu->addSeparator();
    fileMenu->addAction(tr("Save"), [](){}, QKeySequence::Save);
    auto actionSaveAs = fileMenu->addAction(tr("Save as..."), [](){}, QKeySequence(Qt::SHIFT + Qt::CTRL + Qt::Key_S));
    actionSaveAs->setEnabled(false);
    fileMenu->addSeparator();
    fileMenu->addAction(tr("Print"), [](){}, QKeySequence::Print);
    m_FileButton->setMenu(fileMenu);

    const QSize editButtonSize(40, 40);
    const QSize editIconSize(24, 24);

    m_UndoButton = new QPushButton(headerWidget);
    m_UndoButton->setObjectName(QString::fromUtf8("UndoButton"));
    sizePolicy.setHeightForWidth(m_UndoButton->sizePolicy().hasHeightForWidth());
    m_UndoButton->setSizePolicy(sizePolicy);
    m_UndoButton->setFixedSize(editButtonSize);
    m_UndoButton->setIcon(QIcon(":/icons/undo.png"));
    m_UndoButton->setIconSize(editIconSize);

    m_RedoButton = new QPushButton(headerWidget);
    m_RedoButton->setObjectName(QString::fromUtf8("RedoButton"));
    sizePolicy.setHeightForWidth(m_RedoButton->sizePolicy().hasHeightForWidth());
    m_RedoButton->setSizePolicy(sizePolicy);
    m_RedoButton->setFixedSize(editButtonSize);
    m_RedoButton->setIcon(QIcon(":/icons/redo.png"));
    m_RedoButton->setIconSize(editIconSize);

    m_ResetButton = new QPushButton(headerWidget);
    m_ResetButton->setObjectName(QString::fromUtf8("ResetButton"));
    sizePolicy.setHeightForWidth(m_ResetButton->sizePolicy().hasHeightForWidth());
    m_ResetButton->setSizePolicy(sizePolicy);
    m_ResetButton->setFixedSize(editButtonSize);
    m_ResetButton->setIcon(QIcon(":/icons/reset.png"));
    m_ResetButton->setIconSize(editIconSize);

    m_CopyButton = new QPushButton(tr("Copy"), headerWidget);
    m_CopyButton->setObjectName(QString::fromUtf8("CopyButton"));
    m_CopyButton->setFixedSize(QSize(120, 40));
    m_CopyButton->setIconSize(QSize(12, 12));

    QHBoxLayout *headerLayout = new QHBoxLayout(headerWidget);
    headerLayout->setSpacing(12);
    headerLayout->setContentsMargins(12, 12, 24, 12);

    QHBoxLayout *editGroupLayout = new QHBoxLayout();
    editGroupLayout->setSpacing(4);
    editGroupLayout->addWidget(m_UndoButton);
    editGroupLayout->addWidget(m_RedoButton);
    editGroupLayout->addWidget(m_ResetButton);

    headerLayout->addWidget(m_FileButton);
    headerLayout->addLayout(editGroupLayout);
    headerLayout->addItem(new QSpacerItem(40, 20, QSizePolicy::Expanding, QSizePolicy::Minimum));
    headerLayout->addWidget(m_CopyButton);

    headerWidget->setLayout(headerLayout);
    return headerWidget;
}

QScrollArea *MainWindow::createEditArea()
{
    QScrollArea* scrollArea = new QScrollArea(centralWidget());
    scrollArea->setObjectName(QString::fromUtf8("ScrollArea"));
    scrollArea->setFrameShape(QFrame::NoFrame);
    scrollArea->setSizeAdjustPolicy(QAbstractScrollArea::AdjustToContents);
    scrollArea->setWidgetResizable(true);
    scrollArea->setAlignment(Qt::AlignCenter);
    m_EditWidget = new QLabel();
    m_EditWidget->setObjectName(QString::fromUtf8("EditWidget"));
    m_EditWidget->setSizePolicy(QSizePolicy::Maximum, QSizePolicy::Maximum);
    scrollArea->setWidget(m_EditWidget);

    return scrollArea;
}

QWidget *MainWindow::createToolPanel()
{
    QWidget* toolBox = new QWidget(centralWidget());
    toolBox->setFixedWidth(308);
    QVBoxLayout* toolsLayout = new QVBoxLayout(toolBox);
    toolsLayout->setMargin(0);
    toolsLayout->setSpacing(0);
    toolsLayout->addWidget(createToolsButtonsPanel());
    toolsLayout->addWidget(createLine(HorizontalLine, toolBox));
    toolsLayout->addWidget(createOpacityProperty(tr("Opacity image")));
    toolsLayout->addWidget(createColorProperty(tr("Outline color")));
    toolsLayout->addWidget(createLine(HorizontalLine, toolBox));
    toolsLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Minimum, QSizePolicy::Expanding));

    return toolBox;
}

QWidget *MainWindow::createToolsButtonsPanel()
{
    QWidget *toolsWidget = new QWidget(centralWidget());
    toolsWidget->setObjectName(QString::fromUtf8("ToolsWidget"));
    toolsWidget->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Preferred);
    toolsWidget->setAutoFillBackground(false);

    QVBoxLayout *toolsVLayout = new QVBoxLayout(toolsWidget);
    toolsVLayout->setContentsMargins(24, 16, 24, 16);

    QLabel* toolsLabel = new QLabel(toolsWidget);
    toolsLabel->setObjectName(QString::fromUtf8("ToolsLabel"));
    toolsLabel->setText(tr("Draw Tools"));
    QFont font;
    font.setFamily(QString::fromUtf8("Roboto"));
    font.setPointSize(16);
    font.setWeight(75);
    toolsLabel->setFont(font);

    QHBoxLayout *toolBoxHLayout = new QHBoxLayout;
    toolBoxHLayout->setMargin(0);
    toolBoxHLayout->setSpacing(4);

    QButtonGroup *buttonGroup = new QButtonGroup(toolsWidget);
    buttonGroup->addButton(createToolsButton(QIcon(":/icons/icon_pencile.png"), toolsWidget));
    buttonGroup->addButton(createToolsButton(QIcon(":/icons/icon_arrow.png"), toolsWidget));
    buttonGroup->addButton(createToolsButton(QIcon(":/icons/icon_rectangle.png"), toolsWidget));
    buttonGroup->addButton(createToolsButton(QIcon(":/icons/icon_circle.png"), toolsWidget));
    buttonGroup->addButton(createToolsButton(QIcon(":/icons/icon_triangle.png"), toolsWidget));
    buttonGroup->addButton(createToolsButton(QIcon(":/icons/icon_star.png"), toolsWidget));

    for (const auto button : buttonGroup->buttons())
        toolBoxHLayout->addWidget(button);

    toolsVLayout->addWidget(toolsLabel);
    toolsVLayout->addLayout(toolBoxHLayout);

    return toolsWidget;
}

QPushButton *MainWindow::createToolsButton(QIcon icon, QWidget* parent)
{
    ToolButton *button = new ToolButton(icon, parent);
    button->setCheckable(true);
    button->setFixedSize(QSize(40, 40));
    button->setIconSize(QSize(20, 20));
    button->setCheckedIconColor(QColor("#7BCF28"));

    return button;
}

QWidget *MainWindow::createOpacityProperty(const QString& toolName)
{
    QWidget* container = new QWidget(centralWidget());

    QLabel *toolLabel = new QLabel(toolName, container);
    QFont font;
    font.setFamily(QString::fromUtf8("Roboto"));
    font.setPointSize(14);
    font.setWeight(50);
    toolLabel->setFont(font);

    QSlider* slider = new QSlider(container);
    slider->setObjectName(QString::fromUtf8("OpacitySlider"));
    slider->setMaximum(100);
    slider->setValue(100);
    slider->setOrientation(Qt::Horizontal);

    QLineEdit* sliderEdit = new QLineEdit(container);
    sliderEdit->setObjectName(QString::fromUtf8("lineEdit"));
    sliderEdit->setSizePolicy(QSizePolicy::Fixed, QSizePolicy::Preferred);
    sliderEdit->setMaximumSize(QSize(52, 26));
    sliderEdit->setFrame(true);
    sliderEdit->setText("100%");
    sliderEdit->setAlignment(Qt::AlignRight);
    sliderEdit->setReadOnly(true);

    QVBoxLayout* containerLayout = new QVBoxLayout(container);
    containerLayout->setSpacing(0);
    containerLayout->setContentsMargins(24, 16, 24, 0);
    containerLayout->addWidget(toolLabel);

    QHBoxLayout* sliderLayout = new QHBoxLayout();
    sliderLayout->setSpacing(14);
    sliderLayout->addWidget(slider);
    sliderLayout->addWidget(sliderEdit);
    containerLayout->addWidget(toolLabel);
    containerLayout->addLayout(sliderLayout);

    connect(slider, &QSlider::valueChanged, [sliderEdit](int value)
    {
        sliderEdit->setText(QString("%1%").arg(value));
    });

    return container;
}

QWidget *MainWindow::createColorProperty(const QString& toolName)
{
    QWidget* container = new QWidget(centralWidget());

    QLabel* toolLabel = new QLabel(toolName, container);
    QFont font(toolLabel->font());
   // font.setFamily(QString::fromUtf8("Roboto"));
    font.setPointSize(14);
    font.setWeight(50);
    toolLabel->setFont(font);

    QPushButton* pipetteButton = new QPushButton(container);
    pipetteButton->setObjectName(QString::fromUtf8("PipetteButton"));
    pipetteButton->setFixedSize(QSize(40, 40));
    pipetteButton->setIcon(QIcon(":/icons/icon_pipette.png"));
    pipetteButton->setIconSize(QSize(20, 20));

    ColorPickButton *colorButton = new ColorPickButton(container);
    colorButton->setObjectName(QString::fromUtf8("ColorPickButton"));
    colorButton->setContentsMargins(2,2,2,2);
    colorButton->setFixedSize(72, 40);

    QHBoxLayout* containerLayout = new QHBoxLayout(container);
    containerLayout->setContentsMargins(24, 16, 24, 16);
    containerLayout->setSpacing(4);
    containerLayout->addWidget(toolLabel);
    containerLayout->addItem(new QSpacerItem(20, 40, QSizePolicy::Expanding, QSizePolicy::Minimum));
    containerLayout->addWidget(pipetteButton);
    containerLayout->addWidget(colorButton);

    connect(colorButton, &QAbstractButton::pressed, this, &MainWindow::showColorDialog);

    return container;
}

QWidget *MainWindow::createLine(MainWindow::LineType lineType, QWidget *parent)
{
    QFrame *line = new QFrame(parent);
    line->setFrameShadow(QFrame::Plain);
    line->setFrameShape(static_cast<QFrame::Shape>(lineType));
    return line;
}
