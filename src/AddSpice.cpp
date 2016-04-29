#include "AddSpice.h"
#include "ui_AddSpice.h"

#include <QStringList>
#include <QApplication>
#include <QFile>
#include <QStringListModel>
#include <QCompleter>
#include <QDebug>
#include <QMessageBox>

#include "AddSpice.h"
#include "XMLWriter.h"
#include "DataFactory.h"
#include "Common.h"

AddSpice::AddSpice(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AddSpice)
{
    ui->setupUi(this);

    connect(ui->pushButton_finish, SIGNAL(clicked()), this, SLOT(finish()));
    connect(ui->pushButton_add, SIGNAL(clicked()), this, SLOT(addContent()));
    connect(ui->pushButton_delete, SIGNAL(clicked()), this, SLOT(deleteContent()));
    connect(ui->pushButton_insert,SIGNAL(clicked()), this, SLOT(insertContent()));

    ui->tableWidget->clear();
    ui->tableWidget->horizontalHeader()->setStretchLastSection(true);
    //ui->tableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    ui->tableWidget->setColumnCount(5);
    ui->tableWidget->setRowCount(0);
    QStringList list;
    list << tr("RT")
         << tr("En")
         << tr("Cn")
         << tr("Value")
         << tr("Content");
    ui->tableWidget->setHorizontalHeaderLabels(list);

    //TODO:temp
    QCompleter *pCompleter = new QCompleter;
    m_pStringListModel = new QStringListModel;
    pCompleter->setModel(m_pStringListModel);
    ui->lineEdit_spiceName->setCompleter(pCompleter);
    connect(ui->lineEdit_spiceName, SIGNAL(textEdited(const QString &)), this, SLOT(spiceNameChanged(const QString &)));
    connect(ui->lineEdit_spiceName, SIGNAL(editingFinished()), this, SLOT(displayResult()));
}

AddSpice::~AddSpice()
{
    delete ui;
}

void AddSpice::closeEvent(QCloseEvent *event)
{
    save();
    QWidget::closeEvent(event);
}

void AddSpice::addContent()
{
    ui->tableWidget->setRowCount(ui->tableWidget->rowCount() + 1);
}

void AddSpice::deleteContent()
{
    ui->tableWidget->removeRow(ui->tableWidget->currentRow());
}

void AddSpice::insertContent()
{
    ui->tableWidget->insertRow(ui->tableWidget->currentRow());
}

void AddSpice::spiceNameChanged(const QString &spiceName)
{
    QStringList textList = DataFactory::instance()->queryHazyText(spiceName,0);

    m_pStringListModel->setStringList(textList);
}

void AddSpice::displayResult()
{
    ui->tableWidget->clear();
    QStringList list;
    list << tr("RT")
         << tr("En")
         << tr("Cn")
         << tr("Value")
         << tr("Content");
    ui->tableWidget->setHorizontalHeaderLabels(list);

    QVector<SpiceContent> vecContent = DataFactory::instance()->queryContentBySpiceName(ui->lineEdit_spiceName->text());
    ui->tableWidget->setRowCount(vecContent.size());
    QTableWidgetItem *pItem = NULL;
    for(int i = 0; i < vecContent.size(); ++i)
    {
        pItem = new QTableWidgetItem(QString::number(vecContent[i].retentionTime, 'f', 2));
        pItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 0, pItem);

        pItem = new QTableWidgetItem(vecContent[i].englishName);
        pItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 1, pItem);

        pItem = new QTableWidgetItem(vecContent[i].chineseName);
        pItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 2, pItem);

        pItem = new QTableWidgetItem(QString::number(vecContent[i].value, 'f', 2));
        pItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 3, pItem);

        pItem = new QTableWidgetItem(QString::number(vecContent[i].content, 'f', 2));
        pItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 4, pItem);
    }
}

void AddSpice::save()
{
    QString strFilePath = qApp->applicationDirPath() + "/data.xml";
    QFile file(strFilePath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << file.error();
        return;
    }
    XMLWriter writer;
    writer.write(&file, DataFactory::instance()->getSpices());
}

void AddSpice::finish()
{
    QString name = ui->lineEdit_spiceName->text();
    if(name.isEmpty())
    {
        return;
    }

    QStringList textList = DataFactory::instance()->queryHazyText(name,0);
    for(int i = 0; i < textList.size(); ++i)
    {
        if(textList.at(i) == name)
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("Warning"));
            msgBox.setInformativeText(tr("the spice has existed,do you want to cover the record?"));
            msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
            msgBox.setDefaultButton(QMessageBox::Ok);
            int ret = msgBox.exec();
            if(ret == QMessageBox::Ok)
            {
                DataFactory::instance()->removeSpice(name);
                break;
            }
            else
            {
                return;
            }
        }
    }

    Spice spice;
    spice.chineseName = name;
    SpiceContent spiceContent;
    for(int i = 0; i < ui->tableWidget->rowCount(); ++i)
    {
        spiceContent.id = i + 1;
        if(ui->tableWidget->item(i, 0) != NULL
                && ui->tableWidget->item(i, 1) != NULL
                && ui->tableWidget->item(i, 2) != NULL
                && ui->tableWidget->item(i, 3) != NULL
                && ui->tableWidget->item(i, 4) != NULL)
        {
            spiceContent.retentionTime = ui->tableWidget->item(i, 0)->text().toDouble();
            spiceContent.englishName = ui->tableWidget->item(i, 1)->text();
            spiceContent.chineseName = ui->tableWidget->item(i, 2)->text();
            spiceContent.value = ui->tableWidget->item(i, 3)->text().toDouble();
            spiceContent.content = ui->tableWidget->item(i, 4)->text().toDouble();
        }
        else
        {
            QMessageBox msgBox;
            msgBox.setWindowTitle(tr("Warning"));
            msgBox.setInformativeText(tr("%1 Row has error, Please check it!").arg(i + 1));
            msgBox.exec();
            return;
        }
        spice.vecContent.push_back(spiceContent);

    }

    DataFactory::instance()->addSpice(spice);

    ui->lineEdit_spiceName->clear();
    ui->tableWidget->setRowCount(0);
}
