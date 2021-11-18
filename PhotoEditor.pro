QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++14

DEFINES += QT_DEPRECATED_WARNINGS

SOURCES += \
    main.cpp \
    MainWindow.cpp \
    widgets/ColorPickButton.cpp \
    widgets/FileMenu.cpp \
    widgets/ToolButton.cpp \
    widgets/WindowTitleBar.cpp

HEADERS += \
    MainWindow.h \
    widgets/ColorPickButton.h \
    widgets/FileMenu.h \
    widgets/ToolButton.h \
    widgets/WindowTitleBar.h

TRANSLATIONS += \
    PhotoEditor_en_US.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    source.qrc
