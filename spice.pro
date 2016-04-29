#-------------------------------------------------
#
# Project created by QtCreator 2016-01-21T18:45:39
#
#-------------------------------------------------

QT       += core gui xml

greaterThan(QT_MAJOR_VERSION, 4): QT += widgets

TARGET = spice
TEMPLATE = app
TRANSLATIONS += spice_cn.ts

INCLUDEPATH += ../include

RC_FILE = spice.rc

FORMS += \
    ui/AddSpice.ui \
    ui/MainWindow.ui

SOURCES += \
    src/AddSpice.cpp \
    src/DataFactory.cpp \
    src/main.cpp \
    src/MainWindow.cpp \
    src/QueryWidget.cpp \
    src/ResultWidget.cpp \
    src/XMLReader.cpp \
    src/XMLWriter.cpp

HEADERS += \
    include/AddSpice.h \
    include/Common.h \
    include/DataFactory.h \
    include/MainWindow.h \
    include/QueryWidget.h \
    include/ResultWidget.h \
    include/XMLReader.h \
    include/XMLWriter.h
