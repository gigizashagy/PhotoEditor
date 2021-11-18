#include "MainWindow.h"

#include <QApplication>
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
#include <QGraphicsDropShadowEffect>

#include <qdebug.h>

#include "widgets/ToolButton.h"
#include "widgets/ColorPickButton.h"
#include "widgets/WindowTitleBar.h"
#include "widgets/FileMenu.h"

#ifdef Q_OS_WIN
#pragma comment (lib, "user32.lib")
#pragma comment (lib, "Dwmapi.lib")
#include <windowsx.h>
#include <dwmapi.h>
#endif

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    m_bJustMaximized(false),
    m_borderWidth(1),
    m_Margins(1,2,1,1)
{
    initWinParams();
    resize(1366, 756);
    setWindowTitle("Photo Editor 1.0");
    setWindowIcon(QIcon(":/icons/PE.png"));
    setCentralWidget(new QWidget(this));
    centralWidget()->setObjectName(QString::fromUtf8("CentralWidget"));
    QVBoxLayout *centralVLayout = new QVBoxLayout(centralWidget());
    centralVLayout->setSpacing(0);
    centralVLayout->setMargin(0);
    centralWidget()->setLayout(centralVLayout);
    setContentsMargins(m_Margins);

    // Title
    m_TitleBar = new WindowTitleBar(this);
    m_TitleBar->setObjectName(QString::fromUtf8("Title"));
    installEventFilter(m_TitleBar);
    centralVLayout->addWidget(m_TitleBar);
    centralVLayout->addWidget(createLine(HorizontalLine, centralWidget()));
    centralVLayout->addWidget(createHeaderBar());
    centralVLayout->addWidget(createLine(HorizontalLine, centralWidget()));
    connect(m_TitleBar, &WindowTitleBar::minimizeButtonPress, this, &MainWindow::onMinimizePress);
    connect(m_TitleBar, &WindowTitleBar::maximizeButtonPress, this, &MainWindow::onMaximizePress);
    connect(m_TitleBar, &WindowTitleBar::closeButtonPress,    this, &MainWindow::onClosePress);

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

void MainWindow::initWinParams()
{
    setWindowFlags(windowFlags() | Qt::Window | Qt::FramelessWindowHint | Qt::WindowSystemMenuHint | Qt::WindowMaximizeButtonHint);

    HWND hwnd = (HWND)this->winId();
    DWORD style = ::GetWindowLong(hwnd, GWL_STYLE);
    ::SetWindowLong(hwnd, GWL_STYLE, style | WS_MAXIMIZEBOX | WS_THICKFRAME | WS_CAPTION);

    const MARGINS shadow = { 5, 5, 5, 5 };
    DwmExtendFrameIntoClientArea(HWND(winId()), &shadow);
}

void MainWindow::showColorDialog()
{
    QColor color = QColorDialog::getColor(Qt::white, nullptr);
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
        m_EditWidget->setPixmap(QPixmap(filePath));
    }
}

void MainWindow::onMinimizePress()
{
    showMinimized();
}

void MainWindow::onMaximizePress()
{
    isMaximized() ? showNormal() : showMaximized();
}

void MainWindow::onClosePress()
{
    close();
}

bool MainWindow::nativeEvent(const QByteArray &eventType, void *message, long *result)
{
    MSG* msg = reinterpret_cast<MSG*>(message);

    switch (msg->message)
    {
    case WM_NCCALCSIZE:
    {
        NCCALCSIZE_PARAMS& params = *reinterpret_cast<NCCALCSIZE_PARAMS*>(msg->lParam);
        if (params.rgrc[0].top != 0)
            params.rgrc[0].top -= 1;
        *result = WVR_REDRAW;

        return true;
    }
    case WM_NCHITTEST:
    {
        *result = 0;

        const LONG border_width = m_borderWidth;
        RECT winrect;
        GetWindowRect(HWND(winId()), &winrect);

        long x = GET_X_LPARAM(msg->lParam);
        long y = GET_Y_LPARAM(msg->lParam);

        if (x >= winrect.left && x < winrect.left + border_width)
            *result = HTLEFT;

        if (x < winrect.right && x >= winrect.right - border_width)
            *result = HTRIGHT;

        if (y < winrect.bottom && y >= winrect.bottom - border_width)
            *result = HTBOTTOM;

        if (y >= winrect.top && y < winrect.top + border_width)
            *result = HTTOP;

        if (x >= winrect.left && x < winrect.left + border_width &&
                y < winrect.bottom && y >= winrect.bottom - border_width)
        {
            *result = HTBOTTOMLEFT;
        }

        if (x < winrect.right && x >= winrect.right - border_width &&
                y < winrect.bottom && y >= winrect.bottom - border_width)
        {
            *result = HTBOTTOMRIGHT;
        }

        if (x >= winrect.left && x < winrect.left + border_width &&
                y >= winrect.top && y < winrect.top + border_width)
        {
            *result = HTTOPLEFT;
        }

        if (x < winrect.right && x >= winrect.right - border_width &&
                y >= winrect.top && y < winrect.top + border_width)
        {
            *result = HTTOPRIGHT;
        }

        if (0 != *result)
            return true;

        const double dpr = this->devicePixelRatioF();
        QPoint pos = m_TitleBar->mapFromGlobal(QPoint(x / dpr, y / dpr));

        if (!m_TitleBar->rect().contains(pos))
            return false;

        QWidget* child = m_TitleBar->childAt(pos);
        if (!child)
        {
            *result = HTCAPTION;
            return true;
        }
        else
        {
            if (m_TitleBar->getWhiteListWidgets().contains(child))
            {
                *result = HTCAPTION;
                return true;
            }
        }
        return false;
    } //end case WM_NCHITTEST
    case WM_GETMINMAXINFO:
    {
        if (::IsZoomed(msg->hwnd))
        {
            RECT frame = { 0, 0, 0, 0 };
            AdjustWindowRect(&frame, WS_OVERLAPPEDWINDOW, WS_MAXIMIZE);
            auto margins = QMargins(frame.left, frame.top, frame.right, frame.bottom) + m_Margins;
            setContentsMargins(margins);
        }
        else
        {
            if (isMaximized())
                setContentsMargins(m_Margins);
        }
        return false;
    }

    default:
        return QMainWindow::nativeEvent(eventType, message, result);
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

    QMenu* fileMenu = new FileMenu(m_FileButton);

    fileMenu->setContentsMargins(6,12,6,6);
    fileMenu->addAction(tr("Open file"), this, &MainWindow::openFileDialog, QKeySequence::Open);
    fileMenu->addSeparator();
    auto actionSave = fileMenu->addAction(tr("Save"), [](){}, QKeySequence::Save);
    actionSave->setEnabled(false);
    fileMenu->addAction(tr("Save as..."), [](){}, QKeySequence(Qt::SHIFT + Qt::CTRL + Qt::Key_S));
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
    QFont font(toolsLabel->font());
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
    buttonGroup->buttons().first()->setChecked(true);

    for (const auto& button : buttonGroup->buttons())
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
    button->setCheckedIconColor(QColor(0x7B, 0xCF, 0x28));

    return button;
}

QWidget *MainWindow::createOpacityProperty(const QString& toolName)
{
    QWidget* container = new QWidget(centralWidget());

    QLabel *toolLabel = new QLabel(toolName, container);
    QFont font(toolLabel->font());
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
