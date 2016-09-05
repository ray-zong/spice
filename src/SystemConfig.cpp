#include "SystemConfig.h"

#include <QApplication>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QFile>
#include <QDebug>

SystemConfig::SystemConfig()
    : m_nMainContentCount(5)
    , m_strImageFilePath(QApplication::applicationDirPath() + "/image")
{
    QString path = QApplication::applicationDirPath() + "/data/config.xml";
    readConfigFile(path);

}

SystemConfig::~SystemConfig()
{

}

void SystemConfig::save()
{
    QString path = QApplication::applicationDirPath() + "/data/config.xml";
    writeConfigFile(path);
}

void SystemConfig::reset()
{
    m_nMainContentCount = 5;
    m_strImageFilePath = QApplication::applicationDirPath() + "/image";
    save();
}

void SystemConfig::readConfigFile(const QString &path)
{
    QFile file(path);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << __FILE__ << __LINE__ << "cannot open file for reading.";
        writeConfigFile(path);
        return;
    }

    QXmlStreamReader reader(&file);

    reader.readNextStartElement();
    if(reader.name() == "Config")
    {
        reader.readNextStartElement();
        m_nMainContentCount = reader.readElementText().toInt();

        //reader.readNextStartElement();
        //m_strImageFilePath = reader.readElementText();
    }
}

void SystemConfig::writeConfigFile(const QString &path)
{
    QFile file(path);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << __FILE__ << __LINE__ << "cannot open file for writing.";
    }

    QXmlStreamWriter writer(&file);

    writer.setAutoFormatting(true);

    writer.writeStartDocument();
    writer.writeStartElement("Config");

    writer.writeTextElement("MainContentCount", QString::number(m_nMainContentCount));
    //writer.writeTextElement("ImageFilePath", m_strImageFilePath);

    writer.writeEndElement();//Config
    writer.writeEndDocument();
}
/*
void SystemConfig::setMainContentCount(int count)
{
    m_nMainContentCount = count;
}

int SystemConfig::getMainContentCount() const
{
    return m_nMainContentCount;
}

void SystemConfig::setImageFilePath(const QString &path)
{
    m_strImageFilePath = path;
}

QString SystemConfig::getImageFilePath() const
{
    return m_strImageFilePath;
}
*/
