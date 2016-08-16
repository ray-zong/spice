#include <QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QDebug>
#include <QFile>

//start widget
#include "SplashWidget.h"
#include "LoginWidget.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    QTextCodec::setCodecForLocale(QTextCodec::codecForName("UTF-8"));

    //display style sheet
    QFile file(":/qss/default.qss");
    if(!file.open(QFile::ReadOnly))
    {
        qDebug() << __FILE__ << __LINE__ << "qss file canot load!";
    }
    QString styleSheet = QLatin1String(file.readAll());
    qApp->setStyleSheet(styleSheet);

    //load plugins
	a.addLibraryPath("./plugins");
    //Qt
    QTranslator translatorQt;
    translatorQt.load(":/translate/qt_zh_CN");
    a.installTranslator(&translatorQt);

    //local
    QTranslator translator;
    translator.load(":/translate/spice_cn");
    a.installTranslator(&translator);

    //SplashWidget splashWidget;
    //splashWidget.show();

    LoginWidget loginWidget;
    loginWidget.show();

    //MainWindow w;
    //w.showMaximized();

    return a.exec();
}
