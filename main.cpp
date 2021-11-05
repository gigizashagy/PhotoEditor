#include "MainWindow.h"

#include <QApplication>
#include <QFile>
#include <QDebug>


int main(int argc, char *argv[])
{
    QCoreApplication::setAttribute(Qt::AA_UseHighDpiPixmaps, true);
    QCoreApplication::setAttribute(Qt::AA_EnableHighDpiScaling, true);

    QApplication app(argc, argv);
    QFile file(":/stylesheets/editor_style_sheet/style.qss");
    file.open(QFile::ReadOnly);
    QString styleSheet = QLatin1String(file.readAll());
    app.setStyleSheet(styleSheet);
    file.close();

    MainWindow window;
    window.resize(1366, 756);
    window.show();

    return app.exec();
}
