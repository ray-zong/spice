#include "OptionDialog.h"
#include "ui_OptionDialog.h"

#include <QFileDialog>
#include <QApplication>

#include "DataFactory.h"
#include "SystemConfig.h"

OptionDialog::OptionDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OptionDialog)
{
    ui->setupUi(this);


    SystemConfig *pSystemConfig = DataFactory::instance()->getSystemConfig();

    //主要成分显示数量
    ui->spinBox_mainContentCount->setValue(pSystemConfig->getMainContentCount());

    //气质图谱文件位置
    ui->lineEdit_imageFilePath->setText(pSystemConfig->getImageFilePath());
    connect(ui->pushButton_imageFilePath, SIGNAL(clicked(bool)), this, SLOT(setImageFilePath(bool)));
    //合理性
    ui->lineEdit_imageFilePath->setReadOnly(true);

    //保存与取消
    connect(ui->pushButton_ok, SIGNAL(clicked(bool)), this, SLOT(saveOption()));
    connect(ui->pushButton_cancel, SIGNAL(clicked(bool)), this, SLOT(close()));
}

OptionDialog::~OptionDialog()
{
    delete ui;
}

void OptionDialog::setImageFilePath(bool)
{
    QString dir = QFileDialog::getExistingDirectory(this, tr("Open Directory"),
                                                      QApplication::applicationDirPath(),
                                                      QFileDialog::ShowDirsOnly
                                                      | QFileDialog::DontResolveSymlinks);
    ui->lineEdit_imageFilePath->setText(dir);
}

void OptionDialog::saveOption()
{
    SystemConfig *pSystemConfig = DataFactory::instance()->getSystemConfig();

    pSystemConfig->setMainContentCount(ui->spinBox_mainContentCount->value());
    pSystemConfig->setImageFilePath(ui->lineEdit_imageFilePath->text());

    pSystemConfig->save();
}
