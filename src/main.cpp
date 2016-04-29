#include "MainWindow.h"
#include <QApplication>
#include <QTextCodec>
#include <QTranslator>
#include <QDebug>

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    //QTextCodec::setCodecForTr(QTextCodec::codecForName("GB2312"));
    QTextCodec::setCodecForLocale(QTextCodec::codecForName("GB2312"));
    //QTextCodec::setCodecForCStrings(QTextCodec::codecForName("GB2312"));

    QTranslator translator;
    translator.load(qApp->applicationDirPath() + "/spice_cn");
    a.installTranslator(&translator);

    MainWindow w;
    w.show();

    return a.exec();
}
