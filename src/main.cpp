#include <QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QDebug>

//start widget
#include "SplashWidget.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));

	a.addLibraryPath("./plugins");
    //Qt
    QTranslator translatorQt;
    translatorQt.load(":/translate/qt_zh_CN");
    a.installTranslator(&translatorQt);
    //local
    QTranslator translator;
    translator.load(":/translate/spice_cn");
    a.installTranslator(&translator);

    SplashWidget splashWidget;
    splashWidget.show();

    return a.exec();
}
