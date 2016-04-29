#include "QueryWidget.h"

#include <QHBoxLayout>
#include <QComboBox>
#include <QLineEdit>
#include <QPushButton>
#include <QRadioButton>
#include <QTableWidget>
#include <QStringList>
#include <QMenu>
#include <QAction>
#include <QDebug>
#include <QCompleter>
#include <QStringListModel>


#include "DataFactory.h"
#include "ResultWidget.h"

QueryWidget::QueryWidget(QWidget *parent) :
    QWidget(parent),
    m_pLineEdit_query(NULL),
    m_pPushButton_query(NULL),
    m_pRadioButton_precise(NULL),
    m_pResultWidget(NULL),
    m_pStringListModel(NULL)
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
    connect(m_pLineEdit_query, SIGNAL(editingFinished()), this, SLOT(displayResult()));
    m_pPushButton_query = new QPushButton(tr("Query"));
    connect(m_pPushButton_query, SIGNAL(clicked()), this, SLOT(displayResult()));
    QHBoxLayout *pHLayout_input = new QHBoxLayout;
    pHLayout_input->addStretch();
    pHLayout_input->addWidget(m_pLineEdit_query);
    pHLayout_input->addWidget(m_pPushButton_query);

    //
    m_pRadioButton_precise = new QRadioButton(tr("Spice"));
    m_pRadioButton_precise->setChecked(true);
    QRadioButton *pRadioButton_fuzzy = new QRadioButton(tr("Content"));
    QHBoxLayout *pHLayout_queryType = new QHBoxLayout;
    pHLayout_queryType->addStretch();
    pHLayout_queryType->addWidget(m_pRadioButton_precise);
    pHLayout_queryType->addWidget(pRadioButton_fuzzy);

    m_pResultWidget = new ResultWidget;

    QVBoxLayout *pVLayout = new QVBoxLayout(this);
    pVLayout->addLayout(pHLayout_input);
    pVLayout->addLayout(pHLayout_queryType);
    pVLayout->addWidget(m_pResultWidget);
}

void QueryWidget::displayResult()
{
    if(m_pResultWidget == NULL)
    {
        return;
    }
    int type = 0;
    if(!m_pRadioButton_precise->isChecked())
    {
        type = 1;
    }
    if(m_pResultWidget->display(type, m_pLineEdit_query->text()))
    {
        m_pResultWidget->show();
    }
    //m_pComboBox_query->addItem(m_pComboBox_query->currentText());
}

void QueryWidget::queryTextChanged(const QString &text)
{
    QStringList textList = DataFactory::instance()->queryHazyText(text,
                                                                  m_pRadioButton_precise->isChecked() ? 0 : 1);

    m_pStringListModel->setStringList(textList);
}

