#include "SingleSpiceDialog.h"
#include "ui_SingleSpiceDialog.h"

#include <QAxObject>
#include <QFileDialog>

#include "DataFactory.h"
#include "SpiceInfo.h"

SingleSpiceDialog::SingleSpiceDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SingleSpiceDialog)
    , m_nSpiceId(-1)

{
    ui->setupUi(this);
    QHeaderView *pHeader = ui->tableWidget->horizontalHeader();
    pHeader->setStretchLastSection(true);
    pHeader->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->pushButton_ok, SIGNAL(clicked(bool)), this, SLOT(updateSpice(bool)));
    connect(ui->pushButton_cancel, SIGNAL(clicked(bool)), this, SLOT(close()));

    connect(ui->pushButton_insert, SIGNAL(clicked(bool)), this, SLOT(insertContent(bool)));
    connect(ui->pushButton_delete, SIGNAL(clicked(bool)), this, SLOT(deleteContent(bool)));
    connect(ui->pushButton_add, SIGNAL(clicked(bool)), this, SLOT(addContent(bool)));

    connect(ui->pushButton_import, SIGNAL(clicked(bool)), this, SLOT(importContent(bool)));
    connect(ui->pushButton_export, SIGNAL(clicked(bool)), this, SLOT(exportContent(bool)));

    connect(ui->toolButton_image, SIGNAL(clicked(bool)), this, SLOT(selectImagePath(bool)));
}

SingleSpiceDialog::~SingleSpiceDialog()
{
    delete ui;
}

void SingleSpiceDialog::clearSpice()
{
    //id//
    m_nSpiceId = -1;
    //名称//
    ui->lineEdit_name->clear();
    //中文名称//
    ui->lineEdit_CN->clear();
    //英文名称//
    ui->lineEdit_EN->clear();
    //类型//
    ui->comboBox->setCurrentIndex(0);
    //管理状态
    ui->lineEdit_FEMA->clear();
    ui->lineEdit_FDA->clear();
    ui->lineEdit_COE->clear();
    ui->lineEdit_GB->clear();
    //性状描述
    ui->textEdit_property->clear();
    //感官描述
    ui->textEdit_sense->clear();
    //物理性质
    ui->lineEdit_density->clear();
    ui->lineEdit_refractive->clear();
    ui->lineEdit_solubility->clear();
    //制备提取
    ui->textEdit_extract->clear();
    //主要产地
    ui->textEdit_origin->clear();
    //作用描述
    ui->textEdit_purpose->clear();

    //主要成分
    ui->lineEdit_image->clear();
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
}

void SingleSpiceDialog::setSpice(const SpiceInfoData &spiceInfo)
{
    //id//
    m_nSpiceId = spiceInfo.id;
    //名称//
    QStringList strList = spiceInfo.name.CnList;
    ui->lineEdit_name->setText(strList.at(0));
    strList.removeAt(0);
    //中文名称//
    ui->lineEdit_CN->setText(strList.join(";"));
    //英文名称//
    ui->lineEdit_EN->setText(spiceInfo.name.EnList.join(";"));
    //类型//
    ui->comboBox->setCurrentIndex(spiceInfo.type);
    //管理状态
    ui->lineEdit_FEMA->setText(spiceInfo.management.FEMA);
    ui->lineEdit_FDA->setText(spiceInfo.management.FDA);
    ui->lineEdit_COE->setText(spiceInfo.management.COE);
    ui->lineEdit_GB->setText(spiceInfo.management.GB);
    //性状描述
    ui->textEdit_property->setText(spiceInfo.property);
    //感官描述
    ui->textEdit_sense->setText(spiceInfo.sense);
    //物理性质
    ui->lineEdit_density->setText(spiceInfo.physics.density);
    ui->lineEdit_refractive->setText(spiceInfo.physics.refractive);
    ui->lineEdit_solubility->setText(spiceInfo.physics.solubility);
    //制备提取
    ui->textEdit_extract->setText(spiceInfo.extract);
    //主要产地
    ui->textEdit_origin->setText(spiceInfo.origin);
    //作用描述
    ui->textEdit_purpose->setText(spiceInfo.purpose);

    //主要成分
    ui->lineEdit_image->setText(spiceInfo.imagePath);
    ui->tableWidget->clearContents();
    const QVector<SpiceContent> &vecContent = spiceInfo.vecContent;
    ui->tableWidget->setRowCount(spiceInfo.vecContent.size());

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

        pItem = new QTableWidgetItem(QString::number(vecContent[i].absoluteContent, 'f', 2));
        pItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 3, pItem);

        pItem = new QTableWidgetItem(QString::number(vecContent[i].relativeContent, 'f', 2));
        pItem->setTextAlignment(Qt::AlignCenter);
        ui->tableWidget->setItem(i, 4, pItem);
    }
}

void SingleSpiceDialog::updateSpice(bool)
{
    SpiceInfoData spiceData;
    //id//
    spiceData.id = m_nSpiceId;
    //中文名称//
    spiceData.name.CnList = ui->lineEdit_CN->text().split(";");
    spiceData.name.CnList.insert(0, ui->lineEdit_name->text());
    //英文名称//
    spiceData.name.EnList = ui->lineEdit_EN->text().split(";");
    //类型//
    spiceData.type = ui->comboBox->currentIndex();
    //管理状态
    spiceData.management.FEMA = ui->lineEdit_FEMA->text();
    spiceData.management.FDA = ui->lineEdit_FDA->text();
    spiceData.management.COE = ui->lineEdit_COE->text();
    spiceData.management.GB = ui->lineEdit_GB->text();
    //性状描述
    spiceData.property = ui->textEdit_property->toPlainText();
    //感官描述
    spiceData.sense = ui->textEdit_sense->toPlainText();
    //物理性质
    spiceData.physics.density = ui->lineEdit_density->text();
    spiceData.physics.refractive = ui->lineEdit_refractive->text();
    spiceData.physics.solubility = ui->lineEdit_solubility->text();
    //制备提取
    spiceData.extract = ui->textEdit_extract->toPlainText();
    //主要产地
    spiceData.origin = ui->textEdit_origin->toPlainText();
    //作用描述
    spiceData.purpose = ui->textEdit_purpose->toPlainText();

    //主要成分
    spiceData.imagePath = ui->lineEdit_image->text();
    SpiceContent spiceContent;
    for(int i = 0; i < ui->tableWidget->rowCount(); ++i)
    {
        spiceContent.id = i;
        spiceContent.retentionTime = ui->tableWidget->item(i, 0)->text().toDouble();
        spiceContent.englishName = ui->tableWidget->item(i, 1)->text().trimmed();
        spiceContent.chineseName = ui->tableWidget->item(i, 2)->text().trimmed();
        spiceContent.absoluteContent = ui->tableWidget->item(i, 3)->text().toDouble();
        spiceContent.relativeContent = ui->tableWidget->item(i, 4)->text().toDouble();
        spiceData.vecContent.push_back(spiceContent);
    }
    DataFactory::instance()->getSpiceInfo()->updateSpice(spiceData);

    accept();
}

void SingleSpiceDialog::addContent(bool)
{
    ui->tableWidget->insertRow(ui->tableWidget->rowCount());
}

void SingleSpiceDialog::deleteContent(bool)
{
    ui->tableWidget->removeRow(ui->tableWidget->currentRow());
}

void SingleSpiceDialog::insertContent(bool)
{
    ui->tableWidget->insertRow(ui->tableWidget->currentRow());
}

void SingleSpiceDialog::exportContent(bool)
{

}

void SingleSpiceDialog::importContent(bool)
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    //文件选择//
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Excel"), "./", tr("Excel Files (*.xlsx *.xls)"));

    //open the excel
    QAxObject* excel = new QAxObject( "Excel.Application", 0 );
    if(excel == NULL)return;
    QAxObject* workbooks = excel->querySubObject( "Workbooks" );
    if(workbooks == NULL)return;
    QAxObject* workbook = workbooks->querySubObject( "Open(const QString&)", fileName);
    if(workbook == NULL)return;
    QAxObject* sheets = workbook->querySubObject( "Worksheets" );
    if(sheets == NULL) return;
    //QList<QVariantList> data; //Data list from excel, each QVariantList is worksheet row

    //worksheets count
    int count = sheets->dynamicCall("Count()").toInt();

    count = sheets->property("Count").toInt();

    //default read the third sheet
    if(count != 3)
    {
        return;
    }

    QAxObject *sheet = sheets->querySubObject( "Item( int )", 3 );
    QAxObject* rows = sheet->querySubObject( "Rows" );
    int rowCount = rows->dynamicCall( "Count()" ).toInt();
    QAxObject *range = sheet->querySubObject("Cells(int, int)", 1, 1);
    QString value = range->dynamicCall("Value2()").toString();
    QStringList strList = value.split("\\");
    //设置香料名称//
    QString strName = strList.at(strList.size() - 1);
    if(!strName.isEmpty())
        ui->lineEdit_name->setText(strName);

    //second: head
    //unused
    //key:number,return time, english name, chinese name, absolute content, relative content
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
        //when the excel has empty value, there is nothing for using.
        if(value1.isEmpty()
                || value2.isEmpty()
                || value3.isEmpty()
                || value4.isEmpty()
                || value5.isEmpty()
                || value6.isEmpty())
        {
            break;
        }

        addContent(false);
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
}

void SingleSpiceDialog::selectImagePath(bool)
{
    QString fileName = QFileDialog::getOpenFileName(this,
                                                     tr("Open Image"), "./image", tr("Image Files (*.png *.jpg *.bmp)"));
    if(fileName.isEmpty())
        return;

    ui->lineEdit_image->setText(fileName);
}
