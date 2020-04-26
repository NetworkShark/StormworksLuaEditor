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
    dialoghighlight.cpp \
    engine.cpp \
    main.cpp \
    mainwindow.cpp \
    optionsdialog.cpp \
    palettesettingsdialog.cpp

HEADERS += \
    dialoghighlight.h \
    engine.h \
    mainwindow.h \
    optionsdialog.h \
    palettesettingsdialog.h \
    structs.h

FORMS += \
    dialoghighlight.ui \
    mainwindow.ui \
    optionsdialog.ui \
    palettesettingsdialog.ui

TRANSLATIONS += \
    StormworksLuaEditor_it_IT.ts

# Default rules for deployment.
qnx: target.path = /tmp/$${TARGET}/bin
else: unix:!android: target.path = /opt/$${TARGET}/bin
!isEmpty(target.path): INSTALLS += target

RESOURCES += \
    Icons.qrc

win32: LIBS += -L$$PWD/lua535/ -llua5.3.5-static

INCLUDEPATH += $$PWD/lua535
DEPENDPATH += $$PWD/lua535

win32:!win32-g++: PRE_TARGETDEPS += $$PWD/lua535/lua5.3.5-static.lib
else:win32-g++: PRE_TARGETDEPS += $$PWD/lua535/lua5.3.5-static.lib
