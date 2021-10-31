#include "MainWindow.h"

#include <QApplication>
#include <QFile>
#include <QDebug>


int main(int argc, char *argv[])
{
    QApplication app(argc, argv);
    QFile file(":/stylesheets/editor_style_sheet/style.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    app.setStyleSheet(styleSheet);
    app.setAttribute(Qt::AA_EnableHighDpiScaling, true);
    app.setAttribute(Qt::AA_UseHighDpiPixmaps, true);
    file.close();
    MainWindow window;
    window.show();
    return app.exec();
}
