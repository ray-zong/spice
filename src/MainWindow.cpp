#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QApplication>
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

#include "QueryWidget.h"

#include "Common.h"
#include "AddSpice.h"
#include "DeleteSpice.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pAddSpice(NULL),
    m_pDeleteSpice(NULL)
{
    ui->setupUi(this);
    showMaximized();

    QueryWidget *pQueryWidget = new QueryWidget(this);
    setCentralWidget(pQueryWidget);

    m_pAddSpice = new AddSpice;
    m_pDeleteSpice = new DeleteSpice;


    //File Menu
    connect(ui->action_loadFile, SIGNAL(triggered()), SLOT(loadExcelFile()));
    connect(ui->action_exit, SIGNAL(triggered()), qApp, SLOT(quit()));

    //Edit Menu
    connect(ui->action_add, SIGNAL(triggered()), m_pAddSpice, SLOT(showMaximized()));
    connect(ui->action_delete, SIGNAL(triggered()), SLOT(showDeleteSpiceWidget()));

    //View Menu

    //Help Menu
    connect(ui->action_about, SIGNAL(triggered()), SLOT(aboutSoftware()));
}

MainWindow::~MainWindow()
{
    delete ui;
    SAFE_DELETE(m_pAddSpice);
    SAFE_DELETE(m_pDeleteSpice);
}

void MainWindow::closeEvent(QCloseEvent *)
{
    qApp->exit();
}

void MainWindow::loadExcelFile()
{
    if(m_pAddSpice == NULL)
        return;

    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Excel"), ".", tr("Excel Files (*.xlsx *.xls)"));;
    if(fileName.isEmpty())
        return;

    if(!m_pAddSpice->loadExcelFile(fileName))
    {
        QMessageBox msgBox;
        msgBox.setWindowTitle(tr("open excel file"));
        msgBox.setText(tr("the excel file cannot open,please make sure the correct file format!"));
        msgBox.exec();
    }
}

void MainWindow::aboutSoftware()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("About"));
    msgBox.setText(tr("The software is..."));
    msgBox.exec();
}

void MainWindow::showDeleteSpiceWidget()
{
    if(m_pDeleteSpice)
    {
        m_pDeleteSpice->showAllSpice();
        m_pDeleteSpice->showMaximized();
    }
}
