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

DisplaySpice::DisplaySpice(int userType, QWidget *parent)
    : QWidget(parent)
    , m_nUserType(userType)
{
    initTreeWidgetUI();
}

DisplaySpice::~DisplaySpice()
{
}

void DisplaySpice::updateSpice()
{
    if(m_pTreeWidget == NULL)
        return;

    //删除树状列表二级目录//
    m_pTreeWidget->clear();

    //根据香料类型进行分类
    DataFactory *pDataFactory = DataFactory::instance();
    Q_ASSERT(pDataFactory);

    SpiceInfo *pSpiceInfo = pDataFactory->getSpiceInfo();
    Q_ASSERT(pSpiceInfo);

    QTreeWidgetItem *pItem_oils = new QTreeWidgetItem(m_pTreeWidget);
    pItem_oils->setText(0, tr("Oils"));//油类
    QMap<int, QString> mapSpice = pSpiceInfo->allSpice(Oils);

    auto ite = mapSpice.begin();
    for(; ite != mapSpice.end(); ++ite)
    {
         QTreeWidgetItem *pItem = new QTreeWidgetItem(pItem_oils);
         pItem->setData(0, Qt::UserRole, ite.key());
         pItem->setText(0, ite.value());
    }

    QTreeWidgetItem *pItem_tinctures = new QTreeWidgetItem(m_pTreeWidget);
    pItem_tinctures->setText(0, tr("Tinctures"));//酊剂类
    mapSpice = pSpiceInfo->allSpice(Tinctures);
    ite = mapSpice.begin();
    for(; ite != mapSpice.end(); ++ite)
    {
         QTreeWidgetItem *pItem = new QTreeWidgetItem(pItem_tinctures);
         pItem->setData(0, Qt::UserRole, ite.key());
         pItem->setText(0, ite.value());
    }

    QTreeWidgetItem *pItem_concretes = new QTreeWidgetItem(m_pTreeWidget);
    pItem_concretes->setText(0, tr("Concretes"));//浸膏类
    mapSpice = pSpiceInfo->allSpice(Concretes);
    ite = mapSpice.begin();
    for(; ite != mapSpice.end(); ++ite)
    {
         QTreeWidgetItem *pItem = new QTreeWidgetItem(pItem_concretes);
         pItem->setData(0, Qt::UserRole, ite.key());
         pItem->setText(0, ite.value());
    }

    QTreeWidgetItem *pItem_others = new QTreeWidgetItem(m_pTreeWidget);
    pItem_others->setText(0, tr("Others"));//其他
    mapSpice = pSpiceInfo->allSpice(Others);
    ite = mapSpice.begin();
    for(; ite != mapSpice.end(); ++ite)
    {
         QTreeWidgetItem *pItem = new QTreeWidgetItem(pItem_others);
         pItem->setData(0, Qt::UserRole, ite.key());
         pItem->setText(0, ite.value());
    }
}

void DisplaySpice::initTreeWidgetUI()
{
    QVBoxLayout *pVLayout = new QVBoxLayout(this);
    m_pTreeWidget = new QTreeWidget(this);
    pVLayout->addWidget(m_pTreeWidget);
    m_pTreeWidget->setColumnCount(1);
    m_pTreeWidget->header()->setHidden(true);
    if(m_nUserType == Administrator)
    {
        m_pTreeWidget->setContextMenuPolicy(Qt::CustomContextMenu);
        connect(m_pTreeWidget, SIGNAL(customContextMenuRequested(QPoint)), this, SLOT(customContextMenuRequested(QPoint)));
    }
    connect(m_pTreeWidget, SIGNAL(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)), this, SLOT(currentItemChanged(QTreeWidgetItem*,QTreeWidgetItem*)));

    //更新香料内容//
    updateSpice();
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

    QMenu menu(this);
    QAction *pDeleteAction = menu.addAction(QIcon(":/image/delete.png"), tr("Delete"));
    QAction *pAlterAction = menu.addAction(QIcon(":/image/style_edit.png"), tr("Alter"));

    QAction *pAction = menu.exec(QCursor::pos());
    if(pAction == pDeleteAction)
    {
        //删除
        int index = pItem->data(0, Qt::UserRole).toInt();
        emit deleteSpice(index);
    }

    if(pAction == pAlterAction)
    {
        //修改
        emit modifySpice(pItem->data(0, Qt::UserRole).toInt());
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
