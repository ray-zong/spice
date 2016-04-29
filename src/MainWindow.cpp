#include "MainWindow.h"
#include "ui_MainWindow.h"

#include <QApplication>
#include <QDebug>
#include <QMessageBox>

#include "QueryWidget.h"

#include "Common.h"
#include "AddSpice.h"

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),
    m_pAddSpice(NULL)
{
    ui->setupUi(this);

    QueryWidget *pQueryWidget = new QueryWidget;
    setCentralWidget(pQueryWidget);

    m_pAddSpice = new AddSpice;


    //File Menu
    connect(ui->action_loadFile, SIGNAL(triggered()), this, SLOT(loadExcelFile()));
    connect(ui->action_exit, SIGNAL(triggered()), qApp, SLOT(quit()));

    //Edit Menu
    connect(ui->action_add, SIGNAL(triggered()), m_pAddSpice , SLOT(show()));

    //View Menu

    //Help Menu
    connect(ui->action_about, SIGNAL(triggered()), this, SLOT(aboutSoftware()));
}

MainWindow::~MainWindow()
{
    delete ui;
    SAFE_DELETE(m_pAddSpice);
}

void MainWindow::loadExcelFile()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("LoadFile"));
    msgBox.setText(tr("wait..."));
    msgBox.exec();
}

void MainWindow::aboutSoftware()
{
    QMessageBox msgBox;
    msgBox.setWindowTitle(tr("About"));
    msgBox.setText(tr("The software is..."));
    msgBox.exec();
}
