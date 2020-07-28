QT       += core gui

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

CONFIG += c++11

# The following define makes your compiler emit warnings if you use
# any Qt feature that has been marked deprecated (the exact warnings
# depend on your compiler). Please consult the documentation of the
# deprecated API in order to know how to port your code away from it.
DEFINES += QT_DEPRECATED_WARNINGS

# You can also make your code fail to compile if it uses deprecated APIs.
# In order to do so, uncomment the following line.
# You can also select to disable deprecated APIs only up to a certain version of Qt.
#DEFINES += QT_DISABLE_DEPRECATED_BEFORE=0x060000    # disables all the APIs deprecated before Qt 6.0.0

SOURCES += \
    Forms/dialoghighlight.cpp \
    Forms/mainwindow.cpp \
    Forms/optionsdialog.cpp \
    Widgets/cfg_enviroment.cpp \
    engine.cpp \
    highlighter.cpp \
    main.cpp

HEADERS += \
    Forms/dialoghighlight.h \
    Forms/mainwindow.h \
    Forms/optionsdialog.h \
    Widgets/cfg_enviroment.h \
    engine.h \
    highlighter.h \
    structs.h

FORMS += \
    Forms/dialoghighlight.ui \
    Forms/mainwindow.ui \
    Forms/optionsdialog.ui \
    Widgets/cfg_enviroment.ui

TRANSLATIONS += \
    StormworksLuaEditor_it_IT.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Icons.qrc

win32 {
    ## Windows common build here

    contains(QMAKE_HOST.arch, x86_64) {
    ## Windows x64 (64bit) specific build here
    LIBS += -L$$PWD/lua540/ -llua54_x64
    } else {
    ## Windows x86 (32bit) specific build here
    LIBS += -L$$PWD/lua540/ -llua54
    }
}
