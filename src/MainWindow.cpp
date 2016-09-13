#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QTableWidget>
#include <QDockWidget>
#include <QPushButton>
#include <QStatusBar>
#include <QLabel>


#include "Common.h"
#include "DisplaySpice.h"
#include "ResultWidget.h"
#include "QueryWidget.h"
#include "SingleSpiceDialog.h"
#include "DataFactory.h"
#include "SpiceInfo.h"
#include "OptionDialog.h"
#include "UserManagementDialog.h"

MainWindow::MainWindow(const QString &name, int userType, QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_nUserType(userType)
    , m_pLabel_user(NULL)
    , m_pDisplaySpice(NULL)
    , m_pResultWidget(NULL)
    , m_pQueryWidget(NULL)
    , m_pSingleSpiceDialog(NULL)
    , m_pDockWidget(NULL)
    , m_pOptionDialog(NULL)
    , m_pUserManagement(NULL)
    , m_pPushButton_modify(NULL)
    , m_pPushButton_delete(NULL)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/image/spice.ico"));

    //状态栏显示当前用户类型//
    m_pLabel_user = new QLabel(this);
    if(userType == Administrator)
    {
        m_pLabel_user->setText(tr("User:") + name + tr("(Administrator)"));
    }
    else
    {
        m_pLabel_user->setText(tr("User:") + name + tr("(OrdinaryUser)"));
        //屏蔽接口//
        ui->action_add->setDisabled(true);
        ui->action_userManagement->setDisabled(true);
    }
    statusBar()->addWidget(m_pLabel_user);

    initUI();


    //File Menu
    connect(ui->action_exit, SIGNAL(triggered()), qApp, SLOT(quit()));

    //Edit Menu
    connect(ui->action_add, SIGNAL(triggered()), this, SLOT(showNewSpiceWidget()));

    //View Menu
    connect(ui->action_spice, SIGNAL(triggered()), this, SLOT(showAllSpiceWidget()));

    //Tool Menu
    connect(ui->action_userManagement, SIGNAL(triggered()), this, SLOT(showUserManagement()));
    connect(ui->action_option, SIGNAL(triggered()), this, SLOT(showOption()));
    connect(ui->action_transformation, SIGNAL(triggered()), this, SLOT(transformation()));

    //Help Menu
    connect(ui->action_about, SIGNAL(triggered()), SLOT(aboutSoftware()));
}

MainWindow::~MainWindow()
{
    delete ui;
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    emit closeWindow();
    QMainWindow::closeEvent(event);
}

void MainWindow::initUI()
{
    //左侧栏:显示所有香料(树状列表)
    m_pDockWidget = new QDockWidget(tr("Spice"), this);
    m_pDockWidget->setAllowedAreas(Qt::LeftDockWidgetArea | Qt::RightDockWidgetArea);
    m_pDockWidget->setFloating(false);
    //隐藏标题栏
    QWidget * qw=new QWidget(this);
    m_pDockWidget->setTitleBarWidget(qw);

    addDockWidget(Qt::LeftDockWidgetArea, m_pDockWidget);

    m_pDisplaySpice = new DisplaySpice(m_nUserType, this);
    connect(m_pDisplaySpice, SIGNAL(deleteSpice(int)), this, SLOT(deleteSpice(int)));
    connect(m_pDisplaySpice, SIGNAL(modifySpice(int)), this, SLOT(modifySpice(int)));
    connect(m_pDisplaySpice, SIGNAL(showSpice(int)), this, SLOT(showSpice(int)));
    m_pDockWidget->setWidget(m_pDisplaySpice);

    //中心界面
    //右侧栏:底部(结果显示)
    m_pResultWidget = new ResultWidget(this);

    //右侧栏:顶部(查询)
    m_pQueryWidget = new QueryWidget(m_pResultWidget, this);
    connect(m_pQueryWidget, SIGNAL(queryTypeChanged(int)), this, SLOT(queryTypeChanged(int)));

    //修改、删除//
    QHBoxLayout *pHLayout = new QHBoxLayout;
    if(m_nUserType == Administrator)
    {
        m_pPushButton_modify = new QPushButton(tr("Modify"), this);
        connect(m_pPushButton_modify, SIGNAL(clicked(bool)), this, SLOT(modifySpice(bool)));
        m_pPushButton_delete = new QPushButton(tr("Delete"), this);
        connect(m_pPushButton_delete, SIGNAL(clicked(bool)), this, SLOT(deleteSpice(bool)));
        pHLayout->addStretch();
        pHLayout->addWidget(m_pPushButton_modify);
        pHLayout->addWidget(m_pPushButton_delete);
    }


    QVBoxLayout *pVLayout = new QVBoxLayout;
    pVLayout->addWidget(m_pQueryWidget);
    pVLayout->addWidget(m_pResultWidget);
    pVLayout->addLayout(pHLayout);

    QWidget *pCentralWidget = new QWidget(this);
    pCentralWidget->setLayout(pVLayout);

    setCentralWidget(pCentralWidget);
}

void MainWindow::aboutSoftware()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("About"));
    msgBox.setText(tr("The software is..."));
    msgBox.exec();
}

void MainWindow::showNewSpiceWidget()
{
    if(m_pSingleSpiceDialog == NULL)
    {
        m_pSingleSpiceDialog = new SingleSpiceDialog(this);
    }
    m_pSingleSpiceDialog->clearSpice();

    if(m_pSingleSpiceDialog->exec() == QDialog::Accepted)
    {
        m_pDisplaySpice->updateSpice();
    }
}

void MainWindow::showAllSpiceWidget()
{
    QAction *pAction = dynamic_cast<QAction*>(sender());
    if(pAction != NULL)
    {
        if(pAction->isChecked())
        {
            m_pDockWidget->show();
        }
        else
        {
            m_pDockWidget->hide();
        }
    }
}

void MainWindow::deleteSpice(int id)
{
    //弹框请求确认//
    QMessageBox msgBox;
    SpiceInfo* pSpiceInfo = DataFactory::instance()->getSpiceInfo();
    SpiceInfoData spiceInfo;
    if(!pSpiceInfo->findSpice(id, spiceInfo))
    {
        return;
    }
    msgBox.setText(tr("You will delete the spice:%1.").arg(spiceInfo.name.CnList.at(0)));
    msgBox.setInformativeText(tr("Do you want to delete the spice?"));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();

    if(ret == QMessageBox::Ok)
    {
        //若当前显示的香料跟删除的香料为一种，将显示关闭//
        showSpice(-1);
        SpiceInfo* pSpiceInfo = DataFactory::instance()->getSpiceInfo();
        pSpiceInfo->deleteSpice(id);
        if(m_pDisplaySpice != NULL)
        {
            m_pDisplaySpice->updateSpice();
        }
    }
    else if(ret == QMessageBox::Cancel)
    {
        return;
    }
}

void MainWindow::modifySpice(int id)
{
    if(m_pSingleSpiceDialog == NULL)
    {
        m_pSingleSpiceDialog = new SingleSpiceDialog(this);
    }

    SpiceInfo* pSpiceInfo = DataFactory::instance()->getSpiceInfo();
    SpiceInfoData spiceInfo;
    if(pSpiceInfo->findSpice(id, spiceInfo))
    {
        m_pSingleSpiceDialog->setSpice(spiceInfo);
        if(m_pSingleSpiceDialog->exec() == QDialog::Accepted)
        {
            showSpice(id);
            if(m_pDisplaySpice != NULL)
            {
                m_pDisplaySpice->updateSpice();
            }
        }
    }
}

void MainWindow::showSpice(int id)
{
    QVector<SpiceInfoData> vecSpiceInfo;
    SpiceInfo* pSpiceInfo = DataFactory::instance()->getSpiceInfo();
    SpiceInfoData spiceInfo;
    if(pSpiceInfo->findSpice(id, spiceInfo))
    {
        vecSpiceInfo.push_back(spiceInfo);
    }

    m_pResultWidget->displaySpice(vecSpiceInfo);

    //修改查询界面
    if(m_pQueryWidget != NULL)
    {
        m_pQueryWidget->setQueryType(0);
    }
}

void MainWindow::showUserManagement()
{
    if(m_pUserManagement == NULL)
    {
        m_pUserManagement = new UserManagementDialog(this);
    }
    m_pUserManagement->show();
}

void MainWindow::showOption()
{
    if(m_pOptionDialog == NULL)
    {
        m_pOptionDialog = new OptionDialog(this);
    }
    m_pOptionDialog->show();
}

void MainWindow::deleteSpice(bool)
{
    if(m_pResultWidget == NULL)
        return;

    //获取当前id//
    int id = m_pResultWidget->getCurrentSpiceid();
    deleteSpice(id);

    if(m_pDisplaySpice != NULL)
        m_pDisplaySpice->updateSpice();
}

void MainWindow::modifySpice(bool)
{
    if(m_pResultWidget == NULL)
        return;

    //获取当前id//
    int id = m_pResultWidget->getCurrentSpiceid();
    modifySpice(id);

    if(m_pDisplaySpice != NULL)
        m_pDisplaySpice->updateSpice();
}

void MainWindow::queryTypeChanged(int index)
{
    if(m_pPushButton_delete == NULL
            || m_pPushButton_modify == NULL)
        return;

    if(index == 0)
    {
        m_pPushButton_delete->setDisabled(false);
        m_pPushButton_modify->setDisabled(false);
    }
    else
    {
        m_pPushButton_delete->setDisabled(true);
        m_pPushButton_modify->setDisabled(true);
    }
}

#include <QXmlStreamReader>
#include <QFile>
#include <QFileDialog>

void MainWindow::transformation()
{
    SpiceInfo* pSpiceInfo = DataFactory::instance()->getSpiceInfo();

    QString fileName = QFileDialog::getOpenFileName(this, tr("Open XML"), "/home/jana", tr("XML files (*.xml)"));
    QFile file(fileName);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
        return;

    QXmlStreamReader reader(&file);

    //SpiceList
    if(reader.readNextStartElement())
    {
        qDebug() << reader.name();
        if(reader.name() == "SpiceList")
        {
            while(reader.readNextStartElement())
            {
                if(reader.name() == "Spice")
                {
                    SpiceInfoData spiceInfo;
                    spiceInfo.name.CnList.push_back(reader.attributes().value("Cn").toString());
                    while(reader.readNextStartElement())
                    {
                        if(reader.name() == "Content")
                        {
                            SpiceContent spiceContent;
                            spiceContent.id = reader.attributes().value("id").toString().toInt();
                            spiceContent.retentionTime = reader.attributes().value("RT").toString().toDouble();
                            spiceContent.englishName = reader.attributes().value("En").toString().trimmed();
                            spiceContent.chineseName = reader.attributes().value("Cn").toString().trimmed();
                            spiceContent.absoluteContent = reader.attributes().value("Value").toString().toDouble();
                            spiceContent.relativeContent = reader.attributes().value("Content").toString().toDouble();
                            spiceInfo.vecContent.push_back(spiceContent);
                            reader.skipCurrentElement();
                        }
                    }
                    spiceInfo.type = 0;
                    pSpiceInfo->appendSpice(spiceInfo);
                    qDebug() << "insert spice:" << spiceInfo.name.CnList.at(0) << "finish";
                }
                else
                {
                    reader.skipCurrentElement();
                }
            }
        }
    }
    qDebug() << "all finish";
}
