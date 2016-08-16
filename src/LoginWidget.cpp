#include "LoginWidget.h"

#include <QLabel>
#include <QPixmap>
#include <QDebug>
#include <QPushButton>
#include <QMessageBox>
#include <QMouseEvent>
#include <QApplication>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QLineEdit>
#include <QToolButton>

#include "MainWindow.h"

static const int c_nWidth = 400 * 1.5;
static const int c_nHeight = 300 * 1.5;

LoginWidget::LoginWidget()
    : m_pMainWindow(NULL)
    , m_pLineEdit_user(NULL)
    , m_pLineEdit_password(NULL)
{
    setWindowTitle(tr("SpiceDatabaseSystem"));
    setWindowIcon(QIcon(":/image/spice.ico"));

    //hide the title bar
    setWindowFlags(Qt::FramelessWindowHint);
    setAutoFillBackground(true);
    setFixedSize(c_nWidth, c_nHeight);
    setStyleSheet("background-color: AliceBlue");

    //set the logo's image
    QLabel *pLabel = new QLabel(this);
    pLabel->setStyleSheet("background-color: red");
    pLabel->setGeometry(0, 0, width(), height() / 3);
    QPixmap pixmap;
    pixmap.load(":/image/logo.jpg");
    pLabel->setPixmap(pixmap.scaled(pLabel->width(), pLabel->height()));
    setMinAndCloseButton(pLabel);



    //add the title of app
    QLabel *pLabel_title = new QLabel(this);
    pLabel_title->setStyleSheet("color:blue;font-size:25px;font-weight:600;");
    pLabel_title->setText(tr("SpiceDatabaseSystem"));
    pLabel_title->setAlignment(Qt::AlignCenter);
    pLabel_title->setGeometry(width() / 4, pLabel->height(), width() / 2, height() / 6);

    QWidget *pWidget = initLoginWidget();
    pWidget->setParent(this);
    pWidget->setGeometry(width() / 4, pLabel->height() + pLabel_title->height(),
                         width() / 2, height() / 3 * 2 - height() / 4);
    //add tobacoo
    //QLabel *pLabel_tobacco = new QLabel(this);
    //pLabel_tobacco->setStyleSheet("background-color: green");
    //pLabel_tobacco->setGeometry(0, pLabel->height(), width() / 3, (height() - pLabel->height()) / 2);
    //QPixmap pixmap_tobacco;
    //pixmap_tobacco.load(":/image/tobacco.png");
    //pLabel_tobacco->setPixmap(pixmap_tobacco.scaled(pLabel_tobacco->width(), pLabel_tobacco->height()));

    //add spice
    //QLabel *pLabel_spice = new QLabel(this);
    //pLabel_spice->setStyleSheet("background-color: white");
    //pLabel_spice->setGeometry(20, pLabel->height() + pLabel_tobacco->height(), pLabel_tobacco->width(), pLabel_tobacco->height());
    //QPixmap pixmap_spice;
    //pixmap_spice.load(":/image/spice.png");
    //pLabel_spice->setPixmap(pixmap_spice.scaled(pLabel_spice->width() - 40, pLabel_spice->height() - 20));

    //add the title of app
    //QLabel *pLabel_title = new QLabel(this);
    //pLabel_title->setStyleSheet("color:blue;font-size:35px;font-weight:900;");
    //pLabel_title->setText("\n" + tr("Welcome to") + "\n" + tr("SpiceDatabaseSystem"));
    //pLabel_title->setAlignment(Qt::AlignCenter);
    //int x = pLabel_tobacco->width();
    //int y = pLabel->height();
    //int w = width() - pLabel_tobacco->width();
    //int h = height() - pLabel->height();
    //pLabel_title->setGeometry(x, y, w, h / 2);

    //add toolButton:about,start,exit
    //x = pLabel_tobacco->width();
    //y = pLabel->height() + pLabel_title->height();
    //w = width() - pLabel_tobacco->width();
    //h = height() - y;

    //int vSpace = h / 4;
    //int hSpace = vSpace;

    //QPushButton *pPushButton_about = new QPushButton(this);
    //connect(pPushButton_about, SIGNAL(clicked()), SLOT(about()));
    //pPushButton_about->setFocusPolicy(Qt::NoFocus);
    //pPushButton_about->setStyleSheet("QPushButton{image: url(:/image/about.jpg);border:none}" \
    //                                 "QPushButton:hover,pressed{border: 2px solid #8f8f91;}");
    //pPushButton_about->setGeometry(x + hSpace, y + vSpace, 2 * vSpace, 2 * vSpace);

    //QPushButton *pPushButton_start = new QPushButton(this);
    //connect(pPushButton_start, SIGNAL(clicked()), SLOT(start()));
    //pPushButton_start->setFocusPolicy(Qt::NoFocus);
    //pPushButton_start->setStyleSheet("QPushButton{image: url(:/image/start.jpg);border:none}" \
    //                                 "QPushButton:hover,pressed{border: 2px solid #8f8f91;}");
    //pPushButton_start->setGeometry(x + 2 * hSpace + 2 * vSpace, y + vSpace, 2 * vSpace, 2 * vSpace);
    //
    //QPushButton *pPushButton_exit = new QPushButton(this);
    //connect(pPushButton_exit, SIGNAL(clicked()), qApp, SLOT(quit()));
    //pPushButton_exit->setFocusPolicy(Qt::NoFocus);
    //pPushButton_exit->setStyleSheet("QPushButton{image: url(:/image/exit.jpg);border:none}" \
    //                                "QPushButton:hover,pressed{border: 2px solid #8f8f91;}");
    //pPushButton_exit->setGeometry(x + 3 * hSpace + 4 * vSpace, y + vSpace, 2 * vSpace, 2 * vSpace);

    //add PushButton Tip
    //y = pLabel->height() + pLabel_title->height() + vSpace * 3;
    //
    //QLabel *pLabel_about = new QLabel(this);
    //QFont font;
    //font.setPixelSize(15);
    //pLabel_about->setFont(font);
    //pLabel_about->setText(tr("About"));
    //pLabel_about->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    //pLabel_about->setGeometry(x + hSpace, y, vSpace * 2, vSpace);
    //
    //QLabel *pLabel_start = new QLabel(this);
    //pLabel_start->setFont(font);
    //pLabel_start->setText(tr("Start"));
    //pLabel_start->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    //pLabel_start->setGeometry(x + 2 * hSpace + 2 * vSpace, y, vSpace * 2, vSpace);
    //
    //QLabel *pLabel_exit = new QLabel(this);
    //pLabel_exit->setFont(font);
    //pLabel_exit->setText(tr("Exit"));
    //pLabel_exit->setAlignment(Qt::AlignHCenter | Qt::AlignTop);
    //pLabel_exit->setGeometry(x + 3 * hSpace + 4 * vSpace, y, vSpace * 2, vSpace);
}

LoginWidget::~LoginWidget()
{
    if(m_pMainWindow)
    {
        delete m_pMainWindow;
    }
}

void LoginWidget::mousePressEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
    {
        m_bMousePress = true;
    }
    m_movePoint = event->globalPos() - pos();
}

void LoginWidget::mouseReleaseEvent(QMouseEvent *event)
{
    Q_UNUSED(event);
    m_bMousePress = false;
}

void LoginWidget::mouseMoveEvent(QMouseEvent *event)
{
    if(m_bMousePress)
    {
        QPoint movePpos = event->globalPos();
        move(movePpos - m_movePoint);
    }
}

QWidget *LoginWidget::initLoginWidget()
{
   QWidget *pWidget = new QWidget(this);

   QVBoxLayout *pVLayout = new QVBoxLayout(pWidget);

   QHBoxLayout *pHLayout_user = new QHBoxLayout();
   QLabel *pLabel_user = new QLabel;
   pLabel_user->setText(tr("User Name:"));
   m_pLineEdit_user = new QLineEdit;
   m_pLineEdit_user->setPlaceholderText(tr("Please input the user name"));
   pHLayout_user->addWidget(pLabel_user);
   pHLayout_user->addWidget(m_pLineEdit_user);


   QHBoxLayout *pHLayout_password = new QHBoxLayout();
   QLabel *pLabel_password = new QLabel;
   pLabel_password->setText(tr("Password:"));
   m_pLineEdit_password = new QLineEdit;
   m_pLineEdit_password->setEchoMode(QLineEdit::Password);
   m_pLineEdit_password->setPlaceholderText(tr("Please input the password"));

   pHLayout_password->addWidget(pLabel_password);
   pHLayout_password->addWidget(m_pLineEdit_password);

   QHBoxLayout *pHLayout_pushButton = new QHBoxLayout();
   QPushButton *pPushButton_login = new QPushButton;
   pPushButton_login->setText(tr("Login"));
   pPushButton_login->setStyleSheet("QPushButton{background-color:dodgerblue;}" \
                                     "QPushButton:hover,pressed{background-color:deepskyblue;}");

   connect(pPushButton_login, SIGNAL(clicked(bool)), this, SLOT(start()));

   //QPushButton *pPushButton_cancel = new QPushButton;
   //pPushButton_cancel->setText(tr("Cancel"));

   pHLayout_pushButton->addSpacing(125);
   pHLayout_pushButton->addWidget(pPushButton_login);
   //pHLayout_pushButton->addStretch();
   //pHLayout_pushButton->addWidget(pPushButton_cancel);
   pHLayout_pushButton->addSpacing(125);

   pVLayout->addLayout(pHLayout_user);
   pVLayout->addLayout(pHLayout_password);
   pVLayout->addLayout(pHLayout_pushButton);
   //pVLayout->addStretch();

   return pWidget;
}

void LoginWidget::setMinAndCloseButton(QWidget *parent)
{
    //获取界面的宽度
    int width = this->width();
    //设置最小化、关闭按钮在界面的位置
    QToolButton *minBtn = new QToolButton(parent);
    QToolButton *closeBtn = new QToolButton(parent);
    minBtn->setGeometry(width-55,5,20,20);
    closeBtn->setGeometry(width-25,5,20,20);

    //设置鼠标移至按钮上的提示信息
    minBtn->setToolTip(tr("minimum"));
    closeBtn->setToolTip(tr("close"));

    //设置最小化、关闭按钮的样式图标
    minBtn->setIcon(QIcon(":/image/mini.png"));
    minBtn->setStyleSheet("background-color:transparent;");
    closeBtn->setIcon(QIcon(":/image/close.png"));
    closeBtn->setStyleSheet("background-color:transparent;");

    //关联最小化、关闭按钮的槽函数,键盘exe
    connect(minBtn, SIGNAL(clicked()), this, SLOT(slot_minWindow()));
    connect(closeBtn, SIGNAL(clicked()), this, SLOT(slot_closeWindow()));
}

bool LoginWidget::checkUserAndPwd()
{
    if(m_pLineEdit_user == NULL
            || m_pLineEdit_password == NULL)
    {
        return false;
    }

    QString user = m_pLineEdit_user->text();
    QString password = m_pLineEdit_password->text();
    if(user.isEmpty())
    {
        m_pLineEdit_user->setPlaceholderText(tr("The user name cannot be Empty!"));
        return false;
    }

    if(password.isEmpty())
    {
        m_pLineEdit_password->setPlaceholderText(tr("The password cannot be Empty!"));
        return false;
    }

    return true;
}

void LoginWidget::start()
{
    if(!checkUserAndPwd())
    {
        return;
    }

    if(m_pMainWindow == NULL)
    {
        m_pMainWindow = new MainWindow;
        connect(m_pMainWindow, SIGNAL(closeWindow()), this, SLOT(show()));
    }

    m_pMainWindow->show();
    hide();
}

void LoginWidget::slot_minWindow()
{
    this->showMinimized();
}

void LoginWidget::slot_closeWindow()
{
    this->close();
}
