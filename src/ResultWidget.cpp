#include "ResultWidget.h"

#include <QTableWidget>
#include <QVBoxLayout>
#include <QDebug>
#include <QHeaderView>
#include <QTabWidget>
#include <QHeaderView>

#include "BaseInfoWidget.h"
#include "ContentWidget.h"

ResultWidget::ResultWidget(QWidget *parent)
    : QWidget(parent)
    , m_pTabWidget(NULL)
{
    initUI();
    setStyleSheet("QTabWidget{background-color: AliceBlue}");
}

void ResultWidget::initUI()
{
    m_pTabWidget = new QTabWidget(this);
    m_pTabWidget->setTabPosition(QTabWidget::West);
    m_pTabWidget->setTabShape(QTabWidget::Triangular);

    QVBoxLayout *pVLayout = new QVBoxLayout(this);
    pVLayout->addWidget(m_pTabWidget);
}

void ResultWidget::displaySpice(const QVector<SpiceInfoData> &vecSpiceInfo)
{
    if(m_pTabWidget == NULL)
        return;

    //移除所有的Tab//
    for(int i = 0; i < m_pTabWidget->count(); ++i)
    {
        m_pTabWidget->removeTab(i);
    }
    m_vecSpiceId.clear();
    m_pTabWidget->tabBar()->hide();

    //根据查询结果重新添加tab
    auto ite = vecSpiceInfo.begin();
    for(; ite != vecSpiceInfo.end(); ++ite)
    {
        m_vecSpiceId.push_back(ite->id);

        QWidget *pWidget = new QWidget(this);
        if(!ite->name.CnList.isEmpty())
        {
            m_pTabWidget->addTab(pWidget, ite->name.CnList.at(0));
        }

        QTabWidget *pTabWidget = new QTabWidget;
        QHBoxLayout *pHLayout = new QHBoxLayout(pWidget);
        pHLayout->addWidget(pTabWidget);

        //设置基本信息//
        BaseInfoWidget *pBaseInfoWidget = new BaseInfoWidget(this);
        pTabWidget->addTab(pBaseInfoWidget, tr("Base Information"));
        pBaseInfoWidget->setSpiceBaseInfo(*ite);

        //设置主要成分//
        ContentWidget *pContentWidget = new ContentWidget(this);
        pTabWidget->addTab(pContentWidget, tr("Mian Content"));
        pContentWidget->setSpiceContent(*ite);
    }
}

void ResultWidget::displayContent(const QString &name, const QVector<SpiceByContent> &vecSpice)
{
    if(m_pTabWidget == NULL)
        return;

    //移除所有的Tab//
    for(int i = 0; i < m_pTabWidget->count(); ++i)
    {
        m_pTabWidget->removeTab(i);
    }

    m_pTabWidget->tabBar()->hide();

    QTableWidget *pTableWidget = new QTableWidget(this);
    {
        QHeaderView *pHeader = pTableWidget->horizontalHeader();
        pHeader->setStretchLastSection(true);
        pHeader->setSectionResizeMode(QHeaderView::Stretch);

        pHeader->setSortIndicator(0, Qt::AscendingOrder);

        pHeader->setSortIndicatorShown(true);

        connect(pHeader, SIGNAL(sectionClicked(int)), pTableWidget, SLOT(sortByColumn(int)));

        //不可编辑//
        pTableWidget->setEditTriggers(QAbstractItemView::NoEditTriggers);

        //设置列数//
        pTableWidget->setColumnCount(3);
        QStringList list;
        list << tr("Spice Name")
             << tr("Absolute Content(mcg/g)")
             << tr("Relative Content(%)");
        pTableWidget->setHorizontalHeaderLabels(list);

        //设置行数//
        pTableWidget->setRowCount(vecSpice.size());
    }
    m_pTabWidget->addTab(pTableWidget, name);


    MyTableWidgetItem *pMyItem = NULL;
    for(int i = 0; i < vecSpice.size(); ++i)
    {
        QTableWidgetItem *pItem = new QTableWidgetItem(vecSpice.at(i).name.CnList.at(0));
        pItem->setTextAlignment(Qt::AlignCenter);
        pTableWidget->setItem(i, 0, pItem);

        pMyItem = new MyTableWidgetItem;
        pMyItem->setText(QString::number(vecSpice.at(i).absoluteContent, 'f', 2));
        pMyItem->setTextAlignment(Qt::AlignCenter);
        pTableWidget->setItem(i, 1, pMyItem);

        pMyItem = new MyTableWidgetItem;
        pMyItem->setText(QString::number(vecSpice.at(i).relativeContent, 'f', 2));
        pMyItem->setTextAlignment(Qt::AlignCenter);
        pTableWidget->setItem(i, 2, pMyItem);
    }
}

int ResultWidget::getCurrentSpiceid()
{
    return m_vecSpiceId.at(m_pTabWidget->currentIndex());
}
