#-------------------------------------------------
#
# Project created by QtCreator 2016-01-21T18:45:39
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets axcontainer

TARGET = spice
TEMPLATE = app
TRANSLATIONS += resource/translate/spice_cn.ts

INCLUDEPATH += $$PWD/include

FORMS += \
    ui/MainWindow.ui \
    ui/SingleSpiceDialog.ui \
    ui/OptionDialog.ui

SOURCES += \
    src/DataFactory.cpp \
    src/main.cpp \
    src/MainWindow.cpp \
    src/ResultWidget.cpp \
    src/SplashWidget.cpp \
    src/SpiceInfo.cpp \
    src/DisplaySpice.cpp \
    src/BaseInfoWidget.cpp \
    src/ContentWidget.cpp \
    src/QueryWidget.cpp \
    src/SingleSpiceDialog.cpp \
    src/OptionDialog.cpp \
    src/SystemConfig.cpp

HEADERS += \
    include/Common.h \
    include/DataFactory.h \
    include/MainWindow.h \
    include/ResultWidget.h \
    include/SplashWidget.h \
    include/SpiceInfo.h \
    include/DisplaySpice.h \
    include/BaseInfoWidget.h \
    include/ContentWidget.h \
    include/QueryWidget.h \
    include/SingleSpiceDialog.h \
    include/OptionDialog.h \
    include/SystemConfig.h

DISTFILES +=

RESOURCES += \
    resource/spice.qrc
