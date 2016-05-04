#include "AddSpice.h"
#include "ui_AddSpice.h"

#include <QStringList>
#include <QApplication>
#include <QFile>
#include <QStringListModel>
#include <QCompleter>
#include <QDebug>
#include <QMessageBox>
#include <QAxObject>
#include <QModelIndex>

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
    ui->tableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
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

bool AddSpice::loadExcelFile(const QString &fileName)
{
    ui->tableWidget->clearContents();
    QStringList list;
    list << tr("RT")
         << tr("En")
         << tr("Cn")
         << tr("Value")
         << tr("Content");
    ui->tableWidget->setHorizontalHeaderLabels(list);

    //excel读取数据
    QAxObject* excel = new QAxObject( "Excel.Application", 0 );
    QAxObject* workbooks = excel->querySubObject( "Workbooks" );
    QAxObject* workbook = workbooks->querySubObject( "Open(const QString&)", fileName);
    QAxObject* sheets = workbook->querySubObject( "Worksheets" );
    //QList<QVariantList> data; //Data list from excel, each QVariantList is worksheet row

    //worksheets count
    int count = sheets->dynamicCall("Count()").toInt();

    count = sheets->property("Count").toInt();

    //默认直接读取第3个sheet
    if(count != 3)
    {
        return false;
    }

    QAxObject *sheet = sheets->querySubObject( "Item( int )", 3 );
    QAxObject* rows = sheet->querySubObject( "Rows" );
    int rowCount = rows->dynamicCall( "Count()" ).toInt();
    //QAxObject* columns = sheet->querySubObject( "Columns" );
    //int columnCount = columns->property("Count").toInt();
    //qDebug() << "row:" << rowCount << " column:" << columnCount;
    //rowCount = columnCount = 10;
    //第一行为香料的名称
    QAxObject *range = sheet->querySubObject("Cells(int, int)", 1, 1);
    QString value = range->dynamicCall("Value2()").toString();
    QStringList strList = value.split("\\");
    ui->lineEdit_spiceName->setText(strList.at(strList.size() - 1));
    //第二行为表头
    //无效
    //关键数据：默认格式为：序号、生存时间、英文名称、中文名称、绝对含量、相对含量
    for(int i = 3; i <= rowCount; ++i)
    {
        QAxObject *range = sheet->querySubObject("Cells(int, int)", i, 1);
        QString value1 = range->dynamicCall("Value2()").toString().trimmed();
        range = sheet->querySubObject("Cells(int, int)", i, 2);
        QString value2 = range->dynamicCall("Value2()").toString().trimmed();
        range = sheet->querySubObject("Cells(int, int)", i, 3);
        QString value3 = range->dynamicCall("Value2()").toString().trimmed();
        range = sheet->querySubObject("Cells(int, int)", i, 4);
        QString value4 = range->dynamicCall("Value2()").toString().trimmed();
        range = sheet->querySubObject("Cells(int, int)", i, 5);
        QString value5 = range->dynamicCall("Value2()").toString().trimmed();
        range = sheet->querySubObject("Cells(int, int)", i, 6);
        QString value6 = range->dynamicCall("Value2()").toString().trimmed();
        /*qDebug() << value1 << " "
                 << value2 << " "
                 << value3 << " "
                 << value4 << " "
                 << value5 << " " << value6;*/
        //若其中的字段有空值，则表示有效数据读取完成
        if(value1.isEmpty()
                || value2.isEmpty()
                || value3.isEmpty()
                || value4.isEmpty()
                || value5.isEmpty()
                || value6.isEmpty())
        {
            break;
        }

        addContent();
        QTableWidgetItem *pItem = NULL;
        pItem = new QTableWidgetItem(QString::number(value2.toDouble(), 'f', 2));
        pItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i - 3, 0, pItem);

        pItem = new QTableWidgetItem(value3);
        pItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i - 3, 1, pItem);

        pItem = new QTableWidgetItem(value4);
        pItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i - 3, 2, pItem);

        pItem = new QTableWidgetItem(QString::number(value5.toDouble(), 'f', 2));
        pItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i - 3, 3, pItem);

        pItem = new QTableWidgetItem(QString::number(value6.toDouble(), 'f', 2));
        pItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i - 3, 4, pItem);

    }

    workbook->dynamicCall("Close(Boolean)", false);
    excel->dynamicCall("Quit()");
    delete excel;
    ui->tableWidget->setCurrentCell(0, 0);
    this->showMaximized();
    return true;
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
            spiceContent.englishName = ui->tableWidget->item(i, 1)->text().trimmed();
            spiceContent.chineseName = ui->tableWidget->item(i, 2)->text().trimmed();
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
