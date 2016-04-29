#include "XMLWriter.h"

#include <QXmlStreamWriter>

XMLWriter::XMLWriter():
    m_pXmlWriter(NULL)
{
}

XMLWriter::~XMLWriter()
{
    SAFE_DELETE(m_pXmlWriter);
}

bool XMLWriter::write(QIODevice *device, const QVector<Spice> &vecSpice)
{
    if(device == NULL)
        return false;
    if(m_pXmlWriter == NULL)
    {
        m_pXmlWriter = new QXmlStreamWriter;
    }

    m_pXmlWriter->setDevice(device);
    m_pXmlWriter->setAutoFormatting(true);
    m_pXmlWriter->writeStartDocument();
    m_pXmlWriter->writeDTD("<!--RT=RetentionTime En=EnglishName Cn=ChineseName-->");
    m_pXmlWriter->writeStartElement("SpiceList");
    for(int i = 0; i < vecSpice.size(); ++i)
    {
        m_pXmlWriter->writeStartElement("Spice");
        m_pXmlWriter->writeAttribute("Cn", vecSpice[i].chineseName);
        for(int j = 0; j < vecSpice[i].vecContent.size(); ++j)
        {
            m_pXmlWriter->writeStartElement("Content");
            m_pXmlWriter->writeAttribute("id", QString::number(vecSpice[i].vecContent[j].id));
            m_pXmlWriter->writeAttribute("RT", QString::number(vecSpice[i].vecContent[j].retentionTime));
            m_pXmlWriter->writeAttribute("En", vecSpice[i].vecContent[j].englishName.trimmed());
            m_pXmlWriter->writeAttribute("Cn", vecSpice[i].vecContent[j].chineseName.trimmed());
            m_pXmlWriter->writeAttribute("Value", QString::number(vecSpice[i].vecContent[j].value));
            m_pXmlWriter->writeAttribute("Content", QString::number(vecSpice[i].vecContent[j].content));
            m_pXmlWriter->writeEndElement();//Content
        }
        m_pXmlWriter->writeEndElement();//Spice
    }
    m_pXmlWriter->writeEndElement();//SpiceList
    m_pXmlWriter->writeEndDocument();

    return true;
}
