#include "DeleteSpice.h"

#include <QTableWidget>
#include <QVBoxLayout>
#include <QHBoxLayout>
#include <QPushButton>
#include <QHeaderView>
#include <QStringList>
#include <QMessageBox>
#include <QFile>
#include <QApplication>
#include <QDebug>

#include "DataFactory.h"
#include "XMLWriter.h"

DeleteSpice::DeleteSpice(QWidget *parent) :
    QWidget(parent),
    m_pTableWidget(NULL)
{
    initUI();
    showAllSpice();
}

DeleteSpice::~DeleteSpice()
{

}

void DeleteSpice::initUI()
{
    m_pTableWidget = new QTableWidget;
    m_pTableWidget->setSelectionBehavior ( QAbstractItemView::SelectRows); //设置选择行为，以行为单位
    m_pTableWidget->setSelectionMode ( QAbstractItemView::SingleSelection);
    m_pTableWidget->horizontalHeader()->setStretchLastSection(true);

#if (QT_VERSION <= QT_VERSION_CHECK(5, 0, 0))
    m_pTableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
#else
    m_pTableWidget->horizontalHeader()->setSectionResizeMode(QHeaderView::Stretch);
#endif

    QPushButton *pPushButton = new QPushButton(tr("Delete"));
    connect(pPushButton, SIGNAL(clicked()), SLOT(deleteSpice()));
    QHBoxLayout *pHLayout = new QHBoxLayout;
    pHLayout->addStretch();
    pHLayout->addWidget(pPushButton);

    QVBoxLayout *pVLayout = new QVBoxLayout;
    pVLayout->addWidget(m_pTableWidget);
    pVLayout->addLayout(pHLayout);

    setLayout(pVLayout);
}

void DeleteSpice::showAllSpice()
{
    if(m_pTableWidget == NULL)
        return;

    m_pTableWidget->clear();
    m_pTableWidget->setColumnCount(2);
    QStringList list;
    list << tr("Name")
         << tr("Cn");
    m_pTableWidget->setHorizontalHeaderLabels(list);

    QVector<Spice> vecSpice = DataFactory::instance()->getSpices();
    m_pTableWidget->setRowCount(vecSpice.size());
    QTableWidgetItem *pItem = NULL;
    for(int i = 0; i < vecSpice.size(); ++i)
    {
        pItem = new QTableWidgetItem(vecSpice.at(i).chineseName);
        pItem->setTextAlignment(Qt::AlignCenter);
        m_pTableWidget->setItem(i, 0, pItem);

        QStringList contentList;
        const QVector<SpiceContent> &vecContent = vecSpice.at(i).vecContent;
        for(int j = 0; j < vecContent.size(); ++j)
        {
            contentList << vecContent.at(j).chineseName;
        }
        pItem = new QTableWidgetItem(contentList.join(";"));
        pItem->setTextAlignment(Qt::AlignCenter);
        m_pTableWidget->setItem(i, 1, pItem);
    }
}

void DeleteSpice::deleteSpice()
{
    if(m_pTableWidget == NULL)
        return;

    int rowIndex = m_pTableWidget->currentRow();

    QMessageBox msgBox;
    msgBox.setText(tr("Waring!!!"));
    msgBox.setInformativeText(tr("Do you want to delete %1?").arg(m_pTableWidget->item(rowIndex, 0)->text()));
    msgBox.setStandardButtons(QMessageBox::Ok | QMessageBox::Cancel);
    msgBox.setDefaultButton(QMessageBox::Ok);
    int ret = msgBox.exec();
    if(ret == QMessageBox::Ok)
    {
        //Delete
        DataFactory::instance()->removeSpice(rowIndex);
        save();
        showAllSpice();
    }
}

void DeleteSpice::save()
{
    QString strFilePath = qApp->applicationDirPath() + "/data.xml";
    QFile file(strFilePath);
    if(!file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        qDebug() << __FILE__ << __LINE__ << file.error();
        return;
    }
    XMLWriter writer;
    writer.write(&file, DataFactory::instance()->getSpices());
}
