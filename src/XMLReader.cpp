#include "XMLReader.h"

#include <QXmlStreamReader>
#include <QApplication>
#include <QDebug>

#include "DataFactory.h"

XMLReader::XMLReader():
    m_pXmlReader(NULL)
{   
}

XMLReader::~XMLReader()
{
    SAFE_DELETE(m_pXmlReader);
}

bool XMLReader::read(QIODevice *device)
{
    if(device == NULL)
        return false;

    if(m_pXmlReader == NULL)
    {
        m_pXmlReader = new QXmlStreamReader;
    }
    m_pXmlReader->setDevice(device);

    //SpiceList
    if(m_pXmlReader->readNextStartElement())
    {
        qDebug() << m_pXmlReader->name();
        if(m_pXmlReader->name() == "SpiceList")
        {
            readSpice();
        }
    }
    return true;
}

QString XMLReader::errorString() const
{
    if(m_pXmlReader == NULL)
        return "nothing";

    return QObject::tr("%1\nLine %2, column %3")
                .arg(m_pXmlReader->errorString())
                .arg(m_pXmlReader->lineNumber())
                .arg(m_pXmlReader->columnNumber());
}

void XMLReader::readSpice()
{
    while(m_pXmlReader->readNextStartElement())
    {
        if(m_pXmlReader->name() == "Spice")
        {
            Spice spice;
            spice.chineseName = m_pXmlReader->attributes().value("Cn").toString();
            while(m_pXmlReader->readNextStartElement())
            {
                if(m_pXmlReader->name() == "Content")
                {
                    SpiceContent spiceContent;
                    spiceContent.id = m_pXmlReader->attributes().value("id").toString().toInt();
                    spiceContent.retentionTime = m_pXmlReader->attributes().value("RT").toString().toDouble();
                    spiceContent.englishName = m_pXmlReader->attributes().value("En").toString().trimmed();
                    spiceContent.chineseName = m_pXmlReader->attributes().value("Cn").toString().trimmed();
                    spiceContent.value = m_pXmlReader->attributes().value("Value").toString().toDouble();
                    spiceContent.content = m_pXmlReader->attributes().value("Content").toString().toDouble();
                    spice.vecContent.push_back(spiceContent);
                    m_pXmlReader->skipCurrentElement();
                }
            }
            DataFactory::instance()->addSpice(spice);
        }
        else
        {
            m_pXmlReader->skipCurrentElement();
        }
    }
}
