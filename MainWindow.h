#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

class QLabel;
class QPushButton;
class QScrollArea;
class QRadioButton;
class ColorPickButton;
class WindowTitleBar;

class MainWindow : public QMainWindow
{
    Q_OBJECT
    enum LineType
    {
        HorizontalLine = 0x0004, // horizontal line
        VerticalLine = 0x0005 // vertical line
    };

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow() = default;

protected:
    Q_SLOT void showColorDialog();
    Q_SLOT void openFileDialog();

    Q_SLOT void onMinimizePress();
    Q_SLOT void onMaximizePress();
    Q_SLOT void onClosePress();
    // QWidget interface
protected:
    bool nativeEvent(const QByteArray &eventType, void *message, long *result) override;

private:
    void initWinParams();

private:
    QWidget* createHeaderBar();
    QScrollArea* createEditArea();
    QWidget* createToolPanel();
    QWidget* createToolsButtonsPanel();
    QPushButton* createToolsButton(QIcon icon, QWidget* parent = nullptr);
    QWidget* createOpacityProperty(const QString& toolName);
    QWidget* createColorProperty(const QString& toolName);
    QWidget* createLine(LineType lineType, QWidget* parent = nullptr);

private:
    QPushButton* m_FileButton;
    QPushButton* m_UndoButton;
    QPushButton* m_RedoButton;
    QPushButton* m_ResetButton;
    QPushButton* m_CopyButton;
    QLabel* m_EditWidget;
    WindowTitleBar* m_TitleBar;

    bool m_bJustMaximized;
    uint m_borderWidth;
    QMargins m_Margins;
};
#endif // MAINWINDOW_H
