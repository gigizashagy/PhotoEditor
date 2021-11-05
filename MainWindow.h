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
    ~MainWindow();

protected:
    Q_SLOT void showColorDialog();
    Q_SLOT void openFileDialog();

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
};
#endif // MAINWINDOW_H
