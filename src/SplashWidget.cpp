#include "SplashWidget.h"

#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include <QPushButton>
#include <QMessageBox>
#include <QMouseEvent>
#include <QApplication>

#include "MainWindow.h"

static const int c_nWidth = 900;
static const int c_nHeight = 600;

SplashWidget::SplashWidget():
    m_pMainWindow(NULL)
{
    setWindowTitle(tr("SpiceDatabaseSystem"));
    setWindowIcon(QIcon(":/image/spice.ico"));

    //hide the title bar
    setWindowFlags(Qt::FramelessWindowHint);
    setAutoFillBackground(true);
    setFixedSize(c_nWidth, c_nHeight);
    setStyleSheet("background-color: white");

    //set the logo's image
    QLabel *pLabel = new QLabel(this);
    pLabel->setStyleSheet("background-color: red");
    pLabel->setGeometry(0, 0, width(), height() / 4);
    QPixmap pixmap;
    pixmap.load(":/image/logo.jpg");
    //pLabel->setPixmap(pixmap.scaled(pLabel->width(), pLabel->height(), Qt::KeepAspectRatio));
    pLabel->setPixmap(pixmap.scaled(pLabel->width(), pLabel->height()));

    //add tobacoo
    QLabel *pLabel_tobacco = new QLabel(this);
    pLabel_tobacco->setStyleSheet("background-color: green");
    pLabel_tobacco->setGeometry(0, pLabel->height(), width() / 3, (height() - pLabel->height()) / 2);
    QPixmap pixmap_tobacco;
    pixmap_tobacco.load(":/image/tobacco.png");
    pLabel_tobacco->setPixmap(pixmap_tobacco.scaled(pLabel_tobacco->width(), pLabel_tobacco->height()));

    //add spice
    QLabel *pLabel_spice = new QLabel(this);
    pLabel_spice->setStyleSheet("background-color: white");
    pLabel_spice->setGeometry(20, pLabel->height() + pLabel_tobacco->height(), pLabel_tobacco->width(), pLabel_tobacco->height());
    QPixmap pixmap_spice;
    pixmap_spice.load(":/image/spice.png");
    pLabel_spice->setPixmap(pixmap_spice.scaled(pLabel_spice->width() - 40, pLabel_spice->height() - 20));

    //add the title of app
    QLabel *pLabel_title = new QLabel(this);
    pLabel_title->setStyleSheet("color:blue;font-size:35px;font-weight:900;");
    pLabel_title->setText("\n" + tr("Welcome to") + "\n" + tr("SpiceDatabaseSystem"));
    pLabel_title->setAlignment(Qt::AlignCenter);
    int x = pLabel_tobacco->width();
    int y = pLabel->height();
    int w = width() - pLabel_tobacco->width();
    int h = height() - pLabel->height();
    pLabel_title->setGeometry(x, y, w, h / 2);

    //add toolButton:about,start,exit
    x = pLabel_tobacco->width();
    y = pLabel->height() + pLabel_title->height();
    w = width() - pLabel_tobacco->width();
    h = height() - y;

    int vSpace = h / 4;
    int hSpace = vSpace;

    QPushButton *pPushButton_about = new QPushButton(this);
    connect(pPushButton_about, SIGNAL(clicked()), SLOT(about()));
    pPushButton_about->setFocusPolicy(Qt::NoFocus);
    pPushButton_about->setStyleSheet("QPushButton{image: url(:/image/about.jpg);border:none}" \
                                     "QPushButton:hover,pressed{border: 2px solid #8f8f91;}");
    pPushButton_about->setGeometry(x + hSpace, y + vSpace, 2 * vSpace, 2 * vSpace);

    QPushButton *pPushButton_start = new QPushButton(this);
    connect(pPushButton_start, SIGNAL(clicked()), SLOT(start()));
    pPushButton_start->setFocusPolicy(Qt::NoFocus);
    pPushButton_start->setStyleSheet("QPushButton{image: url(:/image/start.jpg);border:none}" \
                                     "QPushButton:hover,pressed{border: 2px solid #8f8f91;}");
    pPushButton_start->setGeometry(x + 2 * hSpace + 2 * vSpace, y + vSpace, 2 * vSpace, 2 * vSpace);

    QPushButton *pPushButton_exit = new QPushButton(this);
    connect(pPushButton_exit, SIGNAL(clicked()), qApp, SLOT(quit()));
    pPushButton_exit->setFocusPolicy(Qt::NoFocus);
    pPushButton_exit->setStyleSheet("QPushButton{image: url(:/image/exit.jpg);border:none}" \
                                    "QPushButton:hover,pressed{border: 2px solid #8f8f91;}");
    pPushButton_exit->setGeometry(x + 3 * hSpace + 4 * vSpace, y + vSpace, 2 * vSpace, 2 * vSpace);

    //add PushButton Tip
    y = pLabel->height() + pLabel_title->height() + vSpace * 3;

    QLabel *pLabel_about = new QLabel(this);
    QFont font;
    font.setPixelSize(15);
    pLabel_about->setFont(font);
    pLabel_about->setText(tr("About"));
    pLabel_about->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    pLabel_about->setGeometry(x + hSpace, y, vSpace * 2, vSpace);

    QLabel *pLabel_start = new QLabel(this);
    pLabel_start->setFont(font);
    pLabel_start->setText(tr("Start"));
    pLabel_start->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    pLabel_start->setGeometry(x + 2 * hSpace + 2 * vSpace, y, vSpace * 2, vSpace);

    QLabel *pLabel_exit = new QLabel(this);
    pLabel_exit->setFont(font);
    pLabel_exit->setText(tr("Exit"));
    pLabel_exit->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    pLabel_exit->setGeometry(x + 3 * hSpace + 4 * vSpace, y, vSpace * 2, vSpace);
}

SplashWidget::~SplashWidget()
{
    if(m_pMainWindow)
    {
        delete m_pMainWindow;
    }
}

void SplashWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_bMousePress = true;
    }
    m_movePoint = event->globalPos() - pos();
}

void SplashWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_bMousePress = false;
}

void SplashWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bMousePress)
    {
        QPoint movePpos = event->globalPos();
        move(movePpos - m_movePoint);
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
        connect(m_pMainWindow, SIGNAL(closeWindow()), this, SLOT(show()));
    }

    m_pMainWindow->show();
    hide();
}
