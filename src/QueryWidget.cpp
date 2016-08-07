#include "QueryWidget.h"

#include <QHBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QComboBox>
#include <QTableWidget>
#include <QStringList>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QCompleter>
#include <QStringListModel>


#include "DataFactory.h"
#include "ResultWidget.h"
#include "SpiceInfo.h"

QueryWidget::QueryWidget(ResultWidget *pResultWidget, QWidget *parent) :
    QWidget(parent),
    m_pLineEdit_query(NULL),
    m_pPushButton_query(NULL),
    m_pComboBox_precise(NULL),
    m_pStringListModel(NULL),
    m_pResultWidget(pResultWidget)
{
    initUI();
}

QueryWidget::~QueryWidget()
{
}

void QueryWidget::initUI()
{
    //Input Query content
    m_pLineEdit_query = new QLineEdit;
    QCompleter *pCompleter = new QCompleter;
    m_pStringListModel = new QStringListModel;
    pCompleter->setModel(m_pStringListModel);
    m_pLineEdit_query->setCompleter(pCompleter);
    connect(m_pLineEdit_query, SIGNAL(textEdited(const QString &)), this, SLOT(queryTextChanged(const QString &)));
    connect(m_pLineEdit_query, SIGNAL(returnPressed()), this, SLOT(displayResult()));
    m_pPushButton_query = new QPushButton(tr("Query"));
    connect(m_pPushButton_query, SIGNAL(clicked()), this, SLOT(displayResult()));

    //
    m_pComboBox_precise = new QComboBox(this);
    m_pComboBox_precise->addItem(tr("Spice"));
    m_pComboBox_precise->addItem(tr("Content"));

    QHBoxLayout *pHLayout_input = new QHBoxLayout(this);
    pHLayout_input->addStretch();
    pHLayout_input->addWidget(m_pComboBox_precise);
    pHLayout_input->addWidget(m_pLineEdit_query);
    pHLayout_input->addWidget(m_pPushButton_query);
    pHLayout_input->addStretch();
}

void QueryWidget::displayResult()
{
    QString text = m_pLineEdit_query->text();
    if(text.isEmpty())
    {
        return;
    }
    SpiceInfo *pSpiceInfo = DataFactory::instance()->getSpiceInfo();

    Q_ASSERT(m_pResultWidget);

    if(m_pComboBox_precise->currentIndex() == 0)
    {
        QVector<SpiceInfoData> vecSpiceInfo = pSpiceInfo->querySpice(text);
        //调用结果显示界面//
        m_pResultWidget->displaySpice(vecSpiceInfo);
    }
    else
    {
        QVector<SpiceByContent> vecSpice = pSpiceInfo->queryContent(text);
        //调用结果显示界面//
        m_pResultWidget->displayContent(text, vecSpice);
    }
}

void QueryWidget::queryTextChanged(const QString &text)
{
    QStringList textList;
    SpiceInfo *pSpiceInfo = DataFactory::instance()->getSpiceInfo();

    if(m_pComboBox_precise->currentIndex() == 0)
    {
        textList = pSpiceInfo->queryHazySpice(text);
    }
    else
    {
        textList = pSpiceInfo->queryHazyContent(text);
    }

    m_pStringListModel->setStringList(textList);
}

