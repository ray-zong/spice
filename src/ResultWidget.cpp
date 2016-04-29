#include "ResultWidget.h"

#include <QTableWidget>
#include <QVBoxLayout>
#include <QDebug>
#include <QHeaderView>

#include "DataFactory.h"

ResultWidget::ResultWidget(QWidget *parent) :
    QWidget(parent),
    m_pTableWidget(NULL)
{
    if(m_pTableWidget == NULL)
    {
        QVBoxLayout *pVLayout = new QVBoxLayout(this);
        m_pTableWidget = new QTableWidget;
        pVLayout->addWidget(m_pTableWidget);

        m_pTableWidget->horizontalHeader()->setStretchLastSection(true);
        //m_pTableWidget->horizontalHeader()->setResizeMode(QHeaderView::Stretch);
    }
}

bool ResultWidget::display(int type, QString query)
{
    if(type == 0)
    {
        return querySpice(query);
    }
    else if(type == 1)
    {
        return querySpiceContent(query);
    }
    else
    {
        Q_ASSERT(false);
        return false;
    }
}

bool ResultWidget::querySpice(const QString &name)
{
    if(m_pTableWidget == NULL)
        return false;

    m_pTableWidget->clear();
    m_pTableWidget->setColumnCount(5);
    QStringList list;
    list << tr("RT")
         << tr("En")
         << tr("Cn")
         << tr("Value")
         << tr("Content");
    m_pTableWidget->setHorizontalHeaderLabels(list);

    QVector<SpiceContent> vecContent = DataFactory::instance()->queryContentBySpiceName(name);
    m_pTableWidget->setRowCount(vecContent.size());
    QTableWidgetItem *pItem = NULL;
    for(int i = 0; i < vecContent.size(); ++i)
    {
        pItem = new QTableWidgetItem(QString::number(vecContent[i].retentionTime, 'f', 2));
        pItem->setTextAlignment(Qt::AlignCenter);
        m_pTableWidget->setItem(i, 0, pItem);

        pItem = new QTableWidgetItem(vecContent[i].englishName);
        pItem->setTextAlignment(Qt::AlignCenter);
        m_pTableWidget->setItem(i, 1, pItem);

        pItem = new QTableWidgetItem(vecContent[i].chineseName);
        pItem->setTextAlignment(Qt::AlignCenter);
        m_pTableWidget->setItem(i, 2, pItem);

        pItem = new QTableWidgetItem(QString::number(vecContent[i].value, 'f', 2));
        pItem->setTextAlignment(Qt::AlignCenter);
        m_pTableWidget->setItem(i, 3, pItem);

        pItem = new QTableWidgetItem(QString::number(vecContent[i].content, 'f', 2));
        pItem->setTextAlignment(Qt::AlignCenter);
        m_pTableWidget->setItem(i, 4, pItem);
    }
    return true;
}

bool ResultWidget::querySpiceContent(const QString &content)
{
    if(m_pTableWidget == NULL)
        return false;

    m_pTableWidget->clear();
    m_pTableWidget->setColumnCount(3);
    QStringList list;
    list << tr("Name")
         << tr("absoluteContent")
         << tr("relativeContent");
    m_pTableWidget->setHorizontalHeaderLabels(list);

    QVector<QStringList> vecList = DataFactory::instance()->querySpiceByContent(content);
    m_pTableWidget->setRowCount(vecList.size());

    QTableWidgetItem *pItem = NULL;
    for(int i = 0; i < vecList.size(); ++i)
    {
        for(int j = 0; j < vecList.at(i).size(); ++j)
        {
            pItem = new QTableWidgetItem(vecList.at(i).at(j));
            pItem->setTextAlignment(Qt::AlignCenter);
            m_pTableWidget->setItem(i, j, pItem);
        }
    }

    return true;
}
