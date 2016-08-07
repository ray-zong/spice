#include "BaseInfoWidget.h"

#include <QTabWidget>
#include <QVBoxLayout>
#include <QTabBar>
#include <QLabel>
#include <QDebug>

BaseInfoWidget::BaseInfoWidget(QWidget *parent)
    : QWidget(parent)
    , m_pTabWidget(NULL)
{
    initUI();
}

BaseInfoWidget::~BaseInfoWidget()
{

}

void BaseInfoWidget::setSpiceBaseInfo(const SpiceInfoData &spiceInfo)
{
    if(m_pTabWidget == NULL || m_pTabWidget->count() != 8)
        return;

    QLabel* pLabel = NULL;
    QString text;
    //中英名称
    {
        pLabel = static_cast<QLabel *>(m_pTabWidget->widget(0));
        text = QString("%1:%2\n%3:%4").arg(tr("chineseName")).arg(spiceInfo.name.CnList.join(";"))
                .arg(tr("englishName")).arg(spiceInfo.name.EnList.join(";"));
        pLabel->setText(text);
    }
    //管理状态
    {
        text.clear();
        pLabel = static_cast<QLabel *>(m_pTabWidget->widget(1));
        if(!spiceInfo.management.FEMA.isEmpty())
        {
            text = QString("%1:%2").arg(tr("FEMA number")).arg(spiceInfo.management.FEMA);
        }
        if(!spiceInfo.management.FDA.isEmpty())
        {
            if(!text.isEmpty())
            {
                text += "\n";
            }
            text += QString("%1:%2").arg(tr("FDA number")).arg(spiceInfo.management.FDA);
        }
        if(!spiceInfo.management.COE.isEmpty())
        {
            if(!text.isEmpty())
            {
                text += "\n";
            }
            text += QString("%1:%2").arg(tr("COE number")).arg(spiceInfo.management.COE);
        }
        if(!spiceInfo.management.GB.isEmpty())
        {
            if(!text.isEmpty())
            {
                text += "\n";
            }
            text += QString("%1:%2").arg(tr("GB")).arg(spiceInfo.management.GB);
        }
        pLabel->setText(text);
    }
    //性状描述
    {
        text.clear();
        pLabel = static_cast<QLabel *>(m_pTabWidget->widget(2));
        text = spiceInfo.property;
        pLabel->setText(text);
    }
    //感官描述
    {
        text.clear();
        pLabel = static_cast<QLabel *>(m_pTabWidget->widget(3));
        text = spiceInfo.sense;
        pLabel->setText(text);
    }
    //物理性质
    {
        text.clear();
        pLabel = static_cast<QLabel *>(m_pTabWidget->widget(4));
        if(!spiceInfo.physics.density.isEmpty())
        {
            text = QString("%1:%2").arg(tr("density")).arg(spiceInfo.physics.density);
        }
        if(!spiceInfo.physics.refractive.isEmpty())
        {
            if(!text.isEmpty())
            {
                text += "\n";
            }
            text += QString("%1:%2").arg(tr("refractive")).arg(spiceInfo.physics.refractive);
        }
        if(!spiceInfo.physics.solubility.isEmpty())
        {
            if(!text.isEmpty())
            {
                text += "\n";
            }
            text += QString("%1:%2").arg(tr("solubility")).arg(spiceInfo.physics.solubility);
        }
        pLabel->setText(text);
    }
    //制备提取
    {
        text.clear();
        pLabel = static_cast<QLabel *>(m_pTabWidget->widget(5));
        text = spiceInfo.extract;
        pLabel->setText(text);
    }
    //主要产地
    {
        text.clear();
        pLabel = static_cast<QLabel *>(m_pTabWidget->widget(6));
        text = spiceInfo.origin;
        pLabel->setText(text);
    }
    //作用描述
    {
        text.clear();
        pLabel = static_cast<QLabel *>(m_pTabWidget->widget(7));
        text = spiceInfo.purpose;
        pLabel->setText(text);
    }
}

void BaseInfoWidget::paintEvent(QPaintEvent *)
{
    if(m_pTabWidget != NULL)
    {
        QString styleSheet = QString("QTabBar::tab{width:%1}").arg(m_pTabWidget->width() / m_pTabWidget->count());
        m_pTabWidget->tabBar()->setStyleSheet(styleSheet);
    }
}

void BaseInfoWidget::initUI()
{
    m_pTabWidget = new QTabWidget(this);

    QLabel *pWidget_0 = new QLabel(this);
    QLabel *pWidget_1 = new QLabel(this);
    QLabel *pWidget_2 = new QLabel(this);
    QLabel *pWidget_3 = new QLabel(this);
    QLabel *pWidget_4 = new QLabel(this);
    QLabel *pWidget_5 = new QLabel(this);
    QLabel *pWidget_6 = new QLabel(this);
    QLabel *pWidget_7 = new QLabel(this);

    m_pTabWidget->addTab(pWidget_0, tr("Name"));
    m_pTabWidget->addTab(pWidget_1, tr("Management"));
    m_pTabWidget->addTab(pWidget_2, tr("Property"));
    m_pTabWidget->addTab(pWidget_3, tr("Sense"));
    m_pTabWidget->addTab(pWidget_4, tr("Physics"));
    m_pTabWidget->addTab(pWidget_5, tr("Extract"));
    m_pTabWidget->addTab(pWidget_6, tr("Origin"));
    m_pTabWidget->addTab(pWidget_7, tr("Purpose"));

    //设置样式
    QFont font;
    font.setPixelSize(20);
    for(int i = 0; i < m_pTabWidget->count(); ++i)
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
