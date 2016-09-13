#include "SingleSpiceDialog.h"
#include "ui_SingleSpiceDialog.h"

#include <QAxObject>
#include <QFileDialog>
#include <QProgressDialog>
#include <QDebug>

#include "DataFactory.h"
#include "SpiceInfo.h"

SingleSpiceDialog::SingleSpiceDialog(QWidget *parent)
    : QDialog(parent)
    , ui(new Ui::SingleSpiceDialog)
    , m_nSpiceId(-1)
    , m_nDialogStatus(NewDialog)
    , m_pProgressDialog(NULL)

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
    //修改对话框状态//
    m_nDialogStatus = NewDialog;
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
    //修改对话框状态//
    m_nDialogStatus = ModifyDialog;
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
    //合法性判断//
    //1.中文名称不能为空//
    if(ui->lineEdit_name->text().isEmpty())
    {
        qDebug() << __FILE__ << __LINE__ << "error";
        return;
    }

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

    //判断是修改数据还是新增数据//
    switch (m_nDialogStatus) {
    case NewDialog:
        DataFactory::instance()->getSpiceInfo()->appendSpice(spiceData);
        break;
    case ModifyDialog:
        DataFactory::instance()->getSpiceInfo()->modifySpice(spiceData);
        break;
    default:
        Q_ASSERT(false);
        break;
    }

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
    QAxObject excel("Excel.Application");
    excel.setProperty("Visible", true);
    QAxObject *work_books = excel.querySubObject("WorkBooks");
    if(work_books == NULL) return;
    //work_books->dynamicCall("Open(const QString&)", QString("D:/Code/spice/bin/debug/test.xlsx"));
    excel.setProperty("Caption", "Qt Excel");
    //QAxObject *work_book = excel.querySubObject("ActiveWorkBook");
    QAxObject *work_book = work_books->querySubObject("Add");
    if(work_book == NULL) return;
    QAxObject *work_sheets = work_book->querySubObject("Sheets");
    if(work_sheets == NULL) return;
    QAxObject *work_sheet = work_sheets->querySubObject("Add");
    if(work_sheet == NULL) return;

    //香料名称//
    QAxObject *cell = work_sheet->querySubObject("Cells(int,int)", 1, 1);
    cell->setProperty("Value", ui->lineEdit_name->text());  //设置单元格值
    //设置列名//
    {
        cell = work_sheet->querySubObject("Cells(int,int)", 2, 1);
        cell->setProperty("Value", "Peak");  //设置单元格值
        cell = work_sheet->querySubObject("Cells(int,int)", 2, 2);
        cell->setProperty("Value", "R.T.");  //设置单元格值
        cell = work_sheet->querySubObject("Cells(int,int)", 2, 3);
        cell->setProperty("Value", "Library/ID");  //设置单元格值
        cell = work_sheet->querySubObject("Cells(int,int)", 2, 4);
        cell->setProperty("Value", QString::fromWCharArray(L"中文名称"));  //设置单元格值
        cell = work_sheet->querySubObject("Cells(int,int)", 2, 5);
        cell->setProperty("Value", QString::fromWCharArray(L"绝对含量μg/g"));  //设置单元格值
        cell = work_sheet->querySubObject("Cells(int,int)", 2, 6);
        cell->setProperty("Value", QString::fromWCharArray(L"相对含量%"));  //设置单元格值
    }

    //香料主要成分//
    for(int i = 0; i < ui->tableWidget->rowCount(); ++i)
    {
        cell = work_sheet->querySubObject("Cells(int,int)", 3 + i, 1);
        cell->setProperty("Value", QString::number(i + 1));  //设置单元格值
        cell = work_sheet->querySubObject("Cells(int,int)", 3 + i, 2);
        cell->setProperty("Value", ui->tableWidget->item(i, 0)->text());  //设置单元格值
        cell = work_sheet->querySubObject("Cells(int,int)", 3 + i, 3);
        cell->setProperty("Value", ui->tableWidget->item(i, 1)->text().trimmed());  //设置单元格值
        cell = work_sheet->querySubObject("Cells(int,int)", 3 + i, 4);
        cell->setProperty("Value", ui->tableWidget->item(i, 2)->text().trimmed());  //设置单元格值
        cell = work_sheet->querySubObject("Cells(int,int)", 3 + i, 5);
        cell->setProperty("Value", ui->tableWidget->item(i, 3)->text());  //设置单元格值
        cell = work_sheet->querySubObject("Cells(int,int)", 3 + i, 6);
        cell->setProperty("Value", ui->tableWidget->item(i, 4)->text());  //设置单元格值
    }

    work_book->dynamicCall("SaveAs (const QString&)", "D:\\Code/spice/bin/debug/test.xlsx");
    work_book->dynamicCall("Close(Boolean)", false);  //关闭文件
    excel.dynamicCall("Quit(void)");  //退出
}

void SingleSpiceDialog::importContent(bool)
{
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);

    //文件选择//
    QString fileName = QFileDialog::getOpenFileName(this,
                                                    tr("Open Excel"), "./", tr("Excel Files (*.xlsx *.xls)"));

//    if(m_pProgressDialog == NULL)
//    {
//        m_pProgressDialog = new QProgressDialog(this);
//        m_pProgressDialog->setWindowTitle(tr("Import/Export Content..."));
//        m_pProgressDialog->setMinimum(0);
//        m_pProgressDialog->setMaximum(0);
//        m_pProgressDialog->setModal(true);
//    }
//    m_pProgressDialog->show();

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
            qDebug() << "error";
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
    excel->dynamicCall("Quit(void)");
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
