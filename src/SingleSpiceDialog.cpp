#include "SingleSpiceDialog.h"
#include "ui_SingleSpiceDialog.h"

#include "DataFactory.h"
#include "SpiceInfo.h"

SingleSpiceDialog::SingleSpiceDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SingleSpiceDialog)
{
    ui->setupUi(this);
    QHeaderView *pHeader = ui->tableWidget->horizontalHeader();
    pHeader->setStretchLastSection(true);
    pHeader->setSectionResizeMode(QHeaderView::Stretch);

    connect(ui->pushButton_ok, SIGNAL(clicked(bool)), this, SLOT(updateSpice(bool)));
    connect(ui->pushButton_cancel, SIGNAL(clicked(bool)), this, SLOT(close()));
}

SingleSpiceDialog::~SingleSpiceDialog()
{
    delete ui;
}

void SingleSpiceDialog::clearSpice()
{
    //名称//
    ui->lineEdit_name->clear();
    //中文名称//
    ui->lineEdit_CN->clear();
    //英文名称//
    ui->lineEdit_EN->clear();
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
    ui->tableWidget->clearContents();
    ui->tableWidget->setRowCount(0);
}

void SingleSpiceDialog::setSpice(const SpiceInfoData &spiceInfo)
{
    //名称//
    ui->lineEdit_name->setText(spiceInfo.name.CnList.at(0));
    //中文名称//
    ui->lineEdit_CN->setText(spiceInfo.name.CnList.join(";"));
    //英文名称//
    ui->lineEdit_EN->setText(spiceInfo.name.EnList.join(";"));
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
    //中文名称//
    //spiceData.name.CnList = ui->lineEdit_CN->text();
    //英文名称//
    //spiceData.name.EnList = ui->lineEdit_EN->text();
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
    SpiceContent spiceContent;
    for(int i = 0; i < ui->tableWidget->rowCount(); ++i)
    {
        spiceContent.id = i;
        spiceContent.retentionTime = ui->tableWidget->item(i, 0)->text().toDouble();
        spiceContent.englishName = ui->tableWidget->item(i, 1)->text().trimmed();
        spiceContent.chineseName = ui->tableWidget->item(i, 2)->text().trimmed();
        spiceContent.absoluteContent = ui->tableWidget->item(i, 3)->text().toDouble();
        spiceContent.relativeContent = ui->tableWidget->item(i, 4)->text().toDouble();
    }
    spiceData.vecContent.push_back(spiceContent);
    DataFactory::instance()->getSpiceInfo()->updateSpice(spiceData);
}
