#-------------------------------------------------
#
# Project created by QtCreator 2016-01-21T18:45:39
#
#-------------------------------------------------

QT       += core gui xml sql

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets axcontainer

TARGET = spice
TEMPLATE = app
TRANSLATIONS += resource/translate/spice_cn.ts
RC_ICONS = resource/image/spice.ico

INCLUDEPATH += include
#DEFINES += QT_NO_CAST_TO_ASCII
QMAKE_LFLAGS_DEBUG += /INCREMENTAL:NO

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
    src/SystemConfig.cpp \
    src/LoginWidget.cpp \
    src/UserManagementDialog.cpp \
    src/DBUser.cpp \
    src/ConnectionPool.cpp

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
    include/SystemConfig.h \
    include/LoginWidget.h \
    include/UserManagementDialog.h \
    include/DBUser.h \
    include/ConnectionPool.h

DISTFILES +=

RESOURCES += \
    resource/spice.qrc
