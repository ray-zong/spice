#include "ContentWidget.h"

#include <QTabWidget>
#include <QVBoxLayout>
#include <QTabBar>
#include <QLabel>
#include <QTableWidget>
#include <QHeaderView>

ContentWidget::ContentWidget(QWidget *parent)
    : QWidget(parent)
    , m_pTabWidget(NULL)
{
    initUI();
}

ContentWidget::~ContentWidget()
{

}

void ContentWidget::setSpiceContent(const SpiceInfoData &spiceInfo)
{
    if(m_pTabWidget == NULL || m_pTabWidget->count() != 3)
        return;

    //气质图谱//
    QLabel* pLabel = static_cast<QLabel *>(m_pTabWidget->widget(0));
    //查找图片//
    QPixmap pixmap;
    pLabel->setPixmap(pixmap);

    //化学成分明细表//
    QString strMainContent;
    QTableWidget* pTableWidget = static_cast<QTableWidget *>(m_pTabWidget->widget(2));
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
        pTableWidget->setColumnCount(5);
        QStringList list;
        list << tr("Retention Time(min)")
             << tr("English Name")
             << tr("Chinese Name")
             << tr("Absolute Content(mcg/g)")
             << tr("Relative Content(%)");
        pTableWidget->setHorizontalHeaderLabels(list);

        //设置行数//
        const QVector<SpiceContent> &vecContent = spiceInfo.vecContent;
        pTableWidget->setRowCount(vecContent.size());

        //设置内容//
        MyTableWidgetItem *pItem = NULL;
        for(int i = 0; i < vecContent.size(); ++i)
        {
            pItem = new MyTableWidgetItem;
            pItem->setText(QString::number(vecContent[i].retentionTime, 'f', 2));
            pItem->setTextAlignment(Qt::AlignCenter);
            pTableWidget->setItem(i, 0, pItem);

            QTableWidgetItem *pQItem = new QTableWidgetItem(vecContent[i].englishName);
            pQItem->setTextAlignment(Qt::AlignCenter);
            pTableWidget->setItem(i, 1, pQItem);

            pQItem = new QTableWidgetItem(vecContent[i].chineseName);
            pQItem->setTextAlignment(Qt::AlignCenter);
            pTableWidget->setItem(i, 2, pQItem);

            pItem = new MyTableWidgetItem;
            pItem->setText(QString::number(vecContent[i].absoluteContent, 'f', 2));
            pItem->setTextAlignment(Qt::AlignCenter);
            pTableWidget->setItem(i, 3, pItem);

            pItem = new MyTableWidgetItem;
            pItem->setText(QString::number(vecContent[i].relativeContent, 'f', 2));
            pItem->setTextAlignment(Qt::AlignCenter);
            pTableWidget->setItem(i, 4, pItem);

            strMainContent += QString("%1(%2%)").arg(vecContent.at(i).chineseName).arg(vecContent.at(i).relativeContent) + tr(";");
        }
    }

    //主要成分//
    pLabel = static_cast<QLabel *>(m_pTabWidget->widget(1));
    QString text = strMainContent.left(strMainContent.size() - 1);
    pLabel->setText(text);
}

void ContentWidget::paintEvent(QPaintEvent *)
{
    if(m_pTabWidget != NULL)
    {
        QString styleSheet = QString("QTabBar::tab{width:%1}").arg(m_pTabWidget->width() / m_pTabWidget->count());
        m_pTabWidget->tabBar()->setStyleSheet(styleSheet);
    }
}

void ContentWidget::initUI()
{
    m_pTabWidget = new QTabWidget(this);

    QLabel *pWidget_0 = new QLabel(this);
    QLabel *pWidget_1 = new QLabel(this);
    QTableWidget *pWidget_2 = new QTableWidget(this);

    m_pTabWidget->addTab(pWidget_0, tr("Image"));
    m_pTabWidget->addTab(pWidget_1, tr("Main Content"));
    m_pTabWidget->addTab(pWidget_2, tr("Table"));

    QFont font;
    font.setPixelSize(20);
    for(int i = 0; i < m_pTabWidget->count() - 1; ++i)
    {
        QLabel* pLabel = static_cast<QLabel *>(m_pTabWidget->widget(i));
        pLabel->setStyleSheet("background-color:AliceBlue");
        pLabel->setAlignment(Qt::AlignCenter);
        pLabel->setWordWrap(true);
        pLabel->setFont(font);
    }

    QVBoxLayout *pVLayout = new QVBoxLayout(this);
    pVLayout->addWidget(m_pTabWidget);
}
