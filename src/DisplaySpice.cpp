#include "DisplaySpice.h"

#include <QTreeWidget>
#include <QVBoxLayout>
#include <QStackedWidget>
#include <QListWidget>
#include <QHeaderView>
#include <QMenu>
#include <QAction>
#include <QDebug>

#include "DataFactory.h"
#include "SpiceInfo.h"

DisplaySpice::DisplaySpice(QWidget *parent)
    : QWidget(parent)
{
    initTreeWidgetUI();
}

DisplaySpice::~DisplaySpice()
{
}

void DisplaySpice::initTreeWidgetUI()
{
    QVBoxLayout *pVLayout = new QVBoxLayout(this);
    m_pTreeWidget = new QTreeWidget(this);
    pVLayout->addWidget(m_pTreeWidget);
    m_pTreeWidget->setColumnCount(1);
    m_pTreeWidget->header()->setHidden(true);
    m_pTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
    connect(m_pTreeWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customContextMenuRequested(QPoint)));
    connect(m_pTreeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));

    QTreeWidgetItem *pItem_oils = new QTreeWidgetItem(m_pTreeWidget);
    pItem_oils->setText(0, tr("Oils"));//油类

    QTreeWidgetItem *pItem_tinctures = new QTreeWidgetItem(m_pTreeWidget);
    pItem_tinctures->setText(0, tr("Tinctures"));//酊剂类

    QTreeWidgetItem *pItem_concretes = new QTreeWidgetItem(m_pTreeWidget);
    pItem_concretes->setText(0, tr("Concretes"));//浸膏类

    QTreeWidgetItem *pItem_others = new QTreeWidgetItem(m_pTreeWidget);
    pItem_others->setText(0, tr("Others"));//其他


    //根据香料类型进行分类
    DataFactory *pDataFactory = DataFactory::instance();
    Q_ASSERT(pDataFactory);

    SpiceInfo *pSpiceInfo = pDataFactory->getSpiceInfo();
    Q_ASSERT(pSpiceInfo);

    QMap<int, SpiceInfoData> mapSpice = pSpiceInfo->allSpice();

    auto ite = mapSpice.begin();
    for(; ite != mapSpice.end(); ++ite)
    {
        QTreeWidgetItem *pItem = NULL;
        switch(ite.value().type)
        {
        case Oils:
            pItem = new QTreeWidgetItem(pItem_oils);
            break;
        case Tinctures:
            pItem = new QTreeWidgetItem(pItem_tinctures);
            break;
        case Concretes:
            pItem = new QTreeWidgetItem(pItem_concretes);
            break;
        case Others:
            pItem = new QTreeWidgetItem(pItem_others);
            break;
        default:
            Q_ASSERT(false);
            break;
        }
        if(pItem)
        {
            if(ite->name.CnList.isEmpty())
            {
                delete pItem;
                Q_ASSERT(false);
            }
            else
            {
                pItem->setData(0, Qt::UserRole, ite->id);
                pItem->setText(0, ite->name.CnList.at(0));
            }
        }
    }
}

void DisplaySpice::customContextMenuRequested(QPoint point)
{
    QTreeWidgetItem *pItem = m_pTreeWidget->itemAt(point);

    if(pItem == NULL)
        return;

    if(pItem->parent() == NULL)
    {
        return;
    }

    //先显示信息
    //emit showSpice(pItem->data(0, Qt::UserRole).toInt());

    QAction *pDeleteAction = new QAction(QIcon(":/image/delete.png"), tr("Delete"), this);
    QAction *pAlterAction = new QAction(QIcon(":/image/style_edit.png"), tr("Alter"), this);

    QMenu *pMenu = new QMenu(this);

    pMenu->addAction(pDeleteAction);
    pMenu->addAction(pAlterAction);

    QAction *pAction = pMenu->exec(QCursor::pos());
    if(pAction == pDeleteAction)
    {
        //删除
        emit deleteSpice(pItem->data(0, Qt::UserRole).toInt());
        delete pItem;
    }

    if(pAction == pAlterAction)
    {
        //修改
        emit alterSpice(pItem->data(0, Qt::UserRole).toInt());
    }
}

void DisplaySpice::currentItemChanged(QTreeWidgetItem *current, QTreeWidgetItem *previous)
{
    Q_UNUSED(previous);
    if(current == NULL)
        return;

    if(current->parent() == NULL)
    {
        return;
    }

    int id = current->data(0, Qt::UserRole).toInt();

    emit showSpice(id);
}
