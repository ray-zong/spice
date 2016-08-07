#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>
#include <QTabWidget>
#include <QHBoxLayout>
#include <QMessageBox>

#include <QTableWidget>
#include <QDockWidget>


#include "Common.h"
#include "DisplaySpice.h"
#include "ResultWidget.h"
#include "QueryWidget.h"
#include "SingleSpiceDialog.h"
#include "DataFactory.h"
#include "SpiceInfo.h"
#include "OptionDialog.h"

MainWindow::MainWindow(QWidget *parent)
    : QMainWindow(parent)
    , ui(new Ui::MainWindow)
    , m_pDisplaySpice(NULL)
    , m_pResultWidget(NULL)
    , m_pQueryWidget(NULL)
    , m_pSingleSpiceDialog(NULL)
    , m_pDockWidget(NULL)
    , m_pOptionDialog(NULL)
{
    ui->setupUi(this);
    setWindowIcon(QIcon(":/image/spice.ico"));

    initUI();


    //File Menu
    connect(ui->action_loadFile, SIGNAL(triggered()), SLOT(loadExcelFile()));
    connect(ui->action_exit, SIGNAL(triggered()), qApp, SLOT(quit()));

    //Edit Menu
    connect(ui->action_add, SIGNAL(triggered()), this, SLOT(showNewSpiceWidget()));

    //View Menu
    connect(ui->action_spice, SIGNAL(triggered()), this, SLOT(showAllSpiceWidget()));

    //Tool Menu
    connect(ui->action_option, SIGNAL(triggered()), this, SLOT(showOption()));

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

    addDockWidget(Qt::LeftDockWidgetArea, m_pDockWidget);

    m_pDisplaySpice = new DisplaySpice(this);
    connect(m_pDisplaySpice, SIGNAL(deleteSpice(int)), this, SLOT(deleteSpice(int)));
    connect(m_pDisplaySpice, SIGNAL(alterSpice(int)), this, SLOT(alterSpice(int)));
    connect(m_pDisplaySpice, SIGNAL(showSpice(int)), this, SLOT(showSpice(int)));
    m_pDockWidget->setWidget(m_pDisplaySpice);

    //中心界面
    //右侧栏:底部(结果显示)
    m_pResultWidget = new ResultWidget(this);

    //右侧栏:顶部(查询)
    m_pQueryWidget = new QueryWidget(m_pResultWidget, this);



    QVBoxLayout *pVLayout = new QVBoxLayout;
    pVLayout->addWidget(m_pQueryWidget);
    pVLayout->addWidget(m_pResultWidget);

    QWidget *pCentralWidget = new QWidget(this);
    pCentralWidget->setLayout(pVLayout);

    setCentralWidget(pCentralWidget);
}

void MainWindow::loadExcelFile()
{
//    if(m_pAddSpice == NULL)
//        return;

//    QString fileName = QFileDialog::getOpenFileName(this,
//                                                    tr("Open Excel"), ".", tr("Excel Files (*.xlsx *.xls)"));;
//    if(fileName.isEmpty())
//        return;

//    if(!m_pAddSpice->loadExcelFile(fileName))
//    {
//        QMessageBox msgBox;
//        msgBox.setWindowTitle(tr("open excel file"));
//        msgBox.setText(tr("the excel file cannot open,please make sure the correct file format!"));
//        msgBox.exec();
//    }
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
    m_pSingleSpiceDialog->show();
}

void MainWindow::showAlterSpiceWidget()
{
    //TODO
    if(m_pSingleSpiceDialog == NULL)
    {
        m_pSingleSpiceDialog = new SingleSpiceDialog(this);
    }
    m_pSingleSpiceDialog->show();
}

void MainWindow::showAllSpiceWidget()
{
    if(m_pDockWidget != NULL)
    {
        m_pDockWidget->show();
    }
}

void MainWindow::deleteSpice(int id)
{
    //弹框请求确认//
    QMessageBox msgBox;
    msgBox.setText("The document has been modified.");
    msgBox.setInformativeText("Do you want to save your changes?");
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();

    if(ret == QMessageBox::Ok)
    {
        //若当前显示的香料跟删除的香料为一种，将显示关闭//
        showSpice(-1);
        SpiceInfo* pSpiceInfo = DataFactory::instance()->getSpiceInfo();
        pSpiceInfo->deleteSpice(id);
    }
    else if(ret == QMessageBox::Cancel)
    {
        return;
    }
}

void MainWindow::alterSpice(int id)
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
        m_pSingleSpiceDialog->show();
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
}

void MainWindow::showOption()
{
    if(m_pOptionDialog == NULL)
    {
        m_pOptionDialog = new OptionDialog(this);
    }
    m_pOptionDialog->show();
}
