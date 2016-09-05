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
#include <QCryptographicHash>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QTimer>

#include "MainWindow.h"
#include "Common.h"

static const int c_nWidth = 400 * 1.5;
static const int c_nHeight = 300 * 1.5;

LoginWidget::LoginWidget()
    : m_pMainWindow(NULL)
    , m_pLineEdit_user(NULL)
    , m_pLineEdit_password(NULL)
    , m_pLabelError(NULL)
    , m_pErrorTipTimer(NULL)
{
    setWindowTitle(tr("SpiceDatabaseSystem"));
    setWindowIcon(QIcon(":/image/spice.ico"));

    readUserAndPwd();

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

    m_pLabelError = new QLabel(this);
    m_pLabelError->setGeometry(0, height() - 25, width(), 25);
    QPalette palette;
    palette.setColor(QPalette::WindowText, QColor(255,0,0));
    m_pLabelError->setPalette(palette);

    m_pErrorTipTimer = new QTimer(this);
    connect(m_pErrorTipTimer, SIGNAL(timeout()), this, SLOT(timeout()));
    //m_pLabelError->setText(tr("login..."));

#ifndef QT_NO_DEBUG
    m_pLineEdit_user->setText("admin");
    m_pLineEdit_password->setText("123456");
#endif
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

   pHLayout_pushButton->addSpacing(125);
   pHLayout_pushButton->addWidget(pPushButton_login);
   pHLayout_pushButton->addSpacing(125);

   pVLayout->addLayout(pHLayout_user);
   pVLayout->addLayout(pHLayout_password);
   pVLayout->addLayout(pHLayout_pushButton);

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

    password = QString::fromUtf8(QCryptographicHash::hash(password.toUtf8(),QCryptographicHash::Md5));

    if(password == m_mapUser[user].password)
    {
        return true;
    }

    return false;
}

void LoginWidget::readUserAndPwd()
{
    QString path = QApplication::applicationDirPath() + "/Data/user.xml";
    QFile file(path);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
              return;

    QXmlStreamReader reader(&file);

    reader.readNextStartElement();
    if(reader.name() == "User")
    {
        while(reader.readNextStartElement())
        {
            User user;
            user.user = reader.attributes().value("user").toString();
            user.password = reader.attributes().value("password").toString();
            user.type = reader.attributes().value("type").toInt() == 0 ? Administrator : OrdinaryUser;

            m_mapUser[user.user] = user;
            reader.skipCurrentElement();
        }
    }
}

void LoginWidget::start()
{
    if(!checkUserAndPwd())
    {
        //提示用户名和密码错误//
        m_pLabelError->setText(tr("The user name or password is wrong"));
        m_pErrorTipTimer->start(1000);
        return;
    }

    if(m_pMainWindow == NULL)
    {
        QString user = m_pLineEdit_user->text();
        m_pMainWindow = new MainWindow(user, m_mapUser[user].type);
        connect(m_pMainWindow, SIGNAL(closeWindow()), this, SLOT(slot_closeWindow()));
    }

    m_pMainWindow->showMaximized();
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

void LoginWidget::timeout()
{
    m_pLabelError->clear();
    m_pErrorTipTimer->stop();
}
