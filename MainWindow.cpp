#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QLayout>
#include <QFile>
#include <QApplication>

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    statusBar()->setSizePolicy(QSizePolicy::Preferred, QSizePolicy::Maximum);
}

MainWindow::~MainWindow()
{
    delete ui;
}
