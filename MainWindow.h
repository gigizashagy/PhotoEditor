#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>

QT_BEGIN_NAMESPACE
namespace Ui { class MainWindow; }
QT_END_NAMESPACE

class ColorPickButton;
class QLabel;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public:
    MainWindow(QWidget *parent = nullptr);
    ~MainWindow();
protected:
    Q_SLOT void showColorDialog();
    Q_SLOT void openFileDialog();
private:
    Ui::MainWindow *ui;

    ColorPickButton* m_ColorButton;
    QLabel* imageWidget;
};
#endif // MAINWINDOW_H
