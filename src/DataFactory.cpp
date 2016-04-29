#include "DataFactory.h"

#include <QApplication>
#include <QString>
#include <QFile>
#include <QDebug>


#include "XMLReader.h"
#include "XMLWriter.h"

DataFactory *DataFactory::m_pDataFactory = NULL;

DataFactory *DataFactory::instance()
{
    if(m_pDataFactory == NULL)
    {
        m_pDataFactory = new DataFactory;
        m_pDataFactory->init();
    }
    return m_pDataFactory;
}

void DataFactory::disinstance()
{
    SAFE_DELETE(m_pDataFactory);
}

const QVector<Spice> &DataFactory::getSpices()
{
    return m_vecSpice;
}

void DataFactory::addSpice(const Spice &spice)
{
    m_vecSpice.push_back(spice);
}

void DataFactory::removeSpice(const QString &spiceName)
{
    for(int i = 0; i < m_vecSpice.size(); ++i)
    {
        if(m_vecSpice.at(i).chineseName.trimmed() == spiceName.trimmed())
        {
            m_vecSpice.remove(i);
            --i;
        }
    }
}

QStringList DataFactory::queryHazyText(const QString &text, int type)
{
    QStringList listText;
    switch(type)
    {
    case 0:
        listText = queryHazySpice(text);
        break;
    case 1:
        listText = queryHazyContent(text);
        break;
    default:
        break;
    }
    return listText;
}

QVector<SpiceContent> DataFactory::queryContentBySpiceName(const QString &spiceName)
{
    QVector<SpiceContent> vecContent;
    for(int i = 0; i < m_vecSpice.size(); ++i)
    {
        if(m_vecSpice[i].chineseName == spiceName)
        {
            vecContent = m_vecSpice[i].vecContent;
        }
    }
    return vecContent;
}

QVector<QStringList> DataFactory::querySpiceByContent(const QString &content)
{
    QVector<QStringList> vecList;
    for(int i = 0; i < m_vecSpice.size(); ++i)
    {
        Spice & spice= m_vecSpice[i];
        for(int j = 0; j < spice.vecContent.size(); ++j)
        {
            if(spice.vecContent[j].chineseName == content
                    || spice.vecContent[j].englishName == content)
            {
                QStringList list;
                list << spice.chineseName;
                list << QString::number(spice.vecContent[j].value, 'f', 2);
                list << QString::number(spice.vecContent[j].content, 'f', 2);
                vecList.push_back(list);
                break;
            }
        }
    }
    return vecList;
}

DataFactory::DataFactory()
{

}

void DataFactory::init()
{
    QString strFilePath = qApp->applicationDirPath() + "/data.xml";
    QFile file(strFilePath);
    if (!file.open(QIODevice::ReadOnly | QIODevice::Text))
    {
        qDebug() << file.error();
        return;
    }

    XMLReader reader;
    reader.read(&file);
}

QStringList DataFactory::queryHazySpice(const QString &text)
{
    qDebug() << text;
    QStringList listText;
    for(int i = 0; i < m_vecSpice.size(); ++i)
    {
        const Spice &spice = m_vecSpice.at(i);
        if(spice.chineseName.indexOf(text) != -1 && listText.indexOf(spice.chineseName) == -1)
        {
            listText << spice.chineseName;
        }
    }
    return listText;
}

QStringList DataFactory::queryHazyContent(const QString &text)
{
    QStringList listText;
    for(int i = 0; i < m_vecSpice.size(); ++i)
    {
        const Spice &spice = m_vecSpice.at(i);
        for(int j = 0; j < spice.vecContent.size(); ++j)
        {
            if(spice.vecContent[j].chineseName.indexOf(text) != -1
                    && listText.indexOf(spice.vecContent[j].chineseName) == -1)
            {
                listText << spice.vecContent[j].chineseName;
            }
            if(spice.vecContent[j].englishName.indexOf(text) != -1
                    && listText.indexOf(spice.vecContent[j].englishName) == -1)
            {
                listText << spice.vecContent[j].englishName;
            }
        }
    }
    return listText;
}

