#include "SplashWidget.h"

#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include <QPushButton>
#include <QMessageBox>
#include <QApplication>

#include "MainWindow.h"

static const int c_nWidth = 900;
static const int c_nHeight = 600;

SplashWidget::SplashWidget():
    m_pMainWindow(NULL)
{
    //hide the title bar
    setWindowFlags(Qt::FramelessWindowHint);
    setAutoFillBackground(true);
    setFixedSize(c_nWidth, c_nHeight);
    setStyleSheet("background-color: white");

    //set the logo's image
    QLabel *pLabel = new QLabel(this);
    pLabel->setStyleSheet("background-color: red");
    pLabel->setGeometry(0, 0, width(), height() / 3);
    QPixmap pixmap;
    pixmap.load(":/splash/image/logo.jpg");
    //pLabel->setPixmap(pixmap.scaled(pLabel->width(), pLabel->height(), Qt::KeepAspectRatio));
    pLabel->setPixmap(pixmap.scaled(pLabel->width(), pLabel->height()));

    //add tobacoo
    QLabel *pLabel_tobacco = new QLabel(this);
    pLabel_tobacco->setStyleSheet("background-color: green");
    pLabel_tobacco->setGeometry(0, pLabel->height(), height() - pLabel->height(), height() - pLabel->height());
    QPixmap pixmap_tobacco;
    pixmap_tobacco.load(":/splash/image/tobacco.png");
    //pLabel_tobacco->setPixmap(pixmap_tobacco.scaled(pLabel_tobacco->width(), pLabel_tobacco->height(), Qt::KeepAspectRatio));
    pLabel_tobacco->setPixmap(pixmap_tobacco.scaled(pLabel_tobacco->width(), pLabel_tobacco->height()));

    //add the title of app
    QLabel *pLabel_title = new QLabel(this);
    QFont font;
    font.setPixelSize(25);
    pLabel_title->setFont(font);
    pLabel_title->setText(tr("SpiceDatabaseSystem"));
    pLabel_title->setAlignment(Qt::AlignCenter);
    int x = pLabel_tobacco->width();
    int y = pLabel->height();
    int w = width() - pLabel_tobacco->width();
    int h = height() - pLabel->height();
    pLabel_title->setGeometry(x, y, w, h / 3);

    //add toolButton:about,start,exit
    x = pLabel_tobacco->width();
    y = pLabel->height() + pLabel_title->height();
    w = width() - pLabel_tobacco->width();
    h = height() - y;

    int vSpace = h / 3;
    int hSpace = (w - h) / 4;

    QPushButton *pPushButton_about = new QPushButton(this);
    connect(pPushButton_about, SIGNAL(clicked()), SLOT(about()));
    pPushButton_about->setStyleSheet("image: url(:/splash/image/about.jpg);");
    pPushButton_about->setGeometry(x + hSpace, y + vSpace, vSpace, vSpace);

    QPushButton *pPushButton_start = new QPushButton(this);
    connect(pPushButton_start, SIGNAL(clicked()), SLOT(start()));
    pPushButton_start->setStyleSheet("image: url(:/splash/image/start.jpg);");
    pPushButton_start->setGeometry(x + 2 * hSpace + vSpace, y + vSpace, vSpace, vSpace);

    QPushButton *pPushButton_exit = new QPushButton(this);
    connect(pPushButton_exit, SIGNAL(clicked()), qApp, SLOT(quit()));
    pPushButton_exit->setStyleSheet("image: url(:/splash/image/exit.jpg);");
    pPushButton_exit->setGeometry(x + 3 * hSpace + 2 * vSpace, y + vSpace, vSpace, vSpace);

    //add toolButton Tip
    y = pLabel->height() + pLabel_title->height() + vSpace * 2;

    QLabel *pLabel_about = new QLabel(this);
    font.setPixelSize(15);
    pLabel_about->setFont(font);
    pLabel_about->setText(tr("About"));
    pLabel_about->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    pLabel_about->setGeometry(x + hSpace, y, vSpace, vSpace);

    QLabel *pLabel_start = new QLabel(this);
    pLabel_start->setFont(font);
    pLabel_start->setText(tr("Start"));
    pLabel_start->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    pLabel_start->setGeometry(x + 2 * hSpace + vSpace, y, vSpace, vSpace);

    QLabel *pLabel_exit = new QLabel(this);
    pLabel_exit->setFont(font);
    pLabel_exit->setText(tr("Exit"));
    pLabel_exit->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    pLabel_exit->setGeometry(x + 3 * hSpace + 2 * vSpace, y, vSpace, vSpace);
}

SplashWidget::~SplashWidget()
{
    if(m_pMainWindow)
    {
        delete m_pMainWindow;
    }
}

void SplashWidget::about()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("About"));
    msgBox.setText(tr("The software is..."));
    msgBox.exec();
}

void SplashWidget::start()
{
    if(m_pMainWindow == NULL)
    {
        m_pMainWindow = new MainWindow;
    }

    m_pMainWindow->show();
    hide();
}
