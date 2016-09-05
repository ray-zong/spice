#include "SpiceInfo.h"

#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QApplication>
#include <QDebug>

SpiceInfo::SpiceInfo(QObject *parent)
    :QObject(parent)
{
    QString path = QApplication::applicationDirPath() + "/data/spiceInfo.xml";
    //读取香料
    readSpiceFile(path);

    path = QApplication::applicationDirPath() + "/data/spiceContent.xml";
    //读取香料成分
    readContentFile(path);
}

SpiceInfo::~SpiceInfo()
{

}

void SpiceInfo::appendSpice(const SpiceInfoData &spiceInfo)
{
    m_mapSpiceInfo[spiceInfo.id] = spiceInfo;
    //保存//
    save();
}

void SpiceInfo::deleteSpice(int id)
{
    auto ite = m_mapSpiceInfo.find(id);
    if(ite != m_mapSpiceInfo.end())
    {
        m_mapSpiceInfo.erase(ite);
    }

    //保存//
    save();
}

void SpiceInfo::updateSpice(SpiceInfoData &spiceInfo)
{
    //新香料//
    if(spiceInfo.id == -1)
    {
        int id = m_mapSpiceInfo.size();

        //为id赋值//
        auto ite = m_mapSpiceInfo.find(id);
        while(ite != m_mapSpiceInfo.end())
        {
            ++id;
            ite = m_mapSpiceInfo.find(id);
        }
        spiceInfo.id = id;
    }

    m_mapSpiceInfo[spiceInfo.id] = spiceInfo;

    //保存//
    save();
}

bool SpiceInfo::findSpice(int id, SpiceInfoData &spiceInfo)
{
    auto ite = m_mapSpiceInfo.find(id);
    if(ite != m_mapSpiceInfo.end())
    {
        spiceInfo = ite.value();
        return true;
    }

    return false;
}

QVector<SpiceInfoData> SpiceInfo::querySpice(const QString &name)
{
    QVector<SpiceInfoData> vecSpiceInfo;
    bool isChinese = name.contains(QRegExp("[\\x4e00-\\x9fa5]+"));
    auto ite = m_mapSpiceInfo.begin();
    for(; ite != m_mapSpiceInfo.end(); ++ite)
    {
        //查找中英文名称//
        const SpiceName &spiceName = ite.value().name;
        if(isChinese)
        {
            //从中文名称中查找//
            for(int i = 0; i < spiceName.CnList.size(); ++i)
            {
                if(spiceName.CnList.at(i).indexOf(name) != -1)
                {
                    vecSpiceInfo.push_back(*ite);
                }
            }
        }
        else
        {
            //从英文名称中查找//
            for(int i = 0; i < spiceName.EnList.size(); ++i)
            {
                if(spiceName.EnList.at(i).indexOf(name) != -1)
                {
                    vecSpiceInfo.push_back(*ite);
                }
            }
        }
    }
    return vecSpiceInfo;
}

QVector<SpiceByContent> SpiceInfo::queryContent(const QString &text)
{
    QVector<SpiceByContent> vecSpice;
    bool isChinese = text.contains(QRegExp("[\\x4e00-\\x9fa5]+"));
    auto ite = m_mapSpiceInfo.begin();
    for(; ite != m_mapSpiceInfo.end(); ++ite)
    {
        QVector<SpiceContent> &vecContent = ite->vecContent;
        for(int i = 0; i < vecContent.size(); ++i)
        {
            SpiceByContent spiceContent;
            //从中文名称中查找//
            if(text.trimmed() == vecContent.at(i).chineseName.trimmed())
            {
                spiceContent.name = ite->name;
                spiceContent.absoluteContent = vecContent.at(i).absoluteContent;
                spiceContent.relativeContent = vecContent.at(i).relativeContent;
                vecSpice.push_back(spiceContent);
                break;
            }

            if(!isChinese)
            {
                //从英文名称中查找//
                if(text.trimmed() == vecContent.at(i).englishName.trimmed())
                {
                    spiceContent.name = ite->name;
                    spiceContent.absoluteContent = vecContent.at(i).absoluteContent;
                    spiceContent.relativeContent = vecContent.at(i).relativeContent;
                    vecSpice.push_back(spiceContent);
                    break;
                }
            }
        }
    }
    return vecSpice;
}

void SpiceInfo::readSpiceFile(const QString &path)
{
    QFile file(path);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << __FILE__ << __LINE__ << "cannot open file.";
    }

    readSpice(&file);
}

void SpiceInfo::readSpice(QIODevice *device)
{
    QXmlStreamReader reader(device);

    reader.readNextStartElement();
    if(reader.name() == "SpiceInfo")
    {

        while(reader.readNextStartElement())
        {
            SpiceInfoData spiceInfo= readSingleSpice(reader);
            m_mapSpiceInfo[spiceInfo.id] = spiceInfo;
            reader.skipCurrentElement();
        }
    }
}

SpiceInfoData SpiceInfo::readSingleSpice(QXmlStreamReader &reader)
{
    SpiceInfoData spiceData;
    //id
    reader.readNextStartElement();
    spiceData.id = reader.readElementText().toInt();
    //名称
    reader.readNextStartElement();
    {
        spiceData.name.EnList = reader.attributes().value("englishName").toString().split(QRegExp("；|;"), QString::SkipEmptyParts);
        spiceData.name.CnList = reader.attributes().value("chineseName").toString().split(QRegExp("、|;|；"), QString::SkipEmptyParts);
    }
    reader.skipCurrentElement();
    //类型
    reader.readNextStartElement();
    spiceData.type = reader.readElementText().toInt();
    //管理状况
    reader.readNextStartElement();
    spiceData.management.FEMA = reader.attributes().value("FEMA").toString();
    spiceData.management.FDA = reader.attributes().value("FDA").toString();
    spiceData.management.COE = reader.attributes().value("COE").toString();
    spiceData.management.GB = reader.attributes().value("GB").toString();
    reader.skipCurrentElement();
    //性状描述
    reader.readNextStartElement();
    spiceData.property = reader.readElementText();
    //感官特征
    reader.readNextStartElement();
    spiceData.sense = reader.readElementText();
    //物理性质
    reader.readNextStartElement();
    spiceData.physics.density = reader.attributes().value("density").toString();
    spiceData.physics.refractive = reader.attributes().value("refractive").toString();
    spiceData.physics.solubility = reader.attributes().value("solubility").toString();
    reader.skipCurrentElement();
    //制备提取
    reader.readNextStartElement();
    spiceData.extract = reader.readElementText();
    //主要产地
    reader.readNextStartElement();
    spiceData.origin = reader.readElementText();
    //作用描述
    reader.readNextStartElement();
    spiceData.purpose = reader.readElementText();
    //气质图谱
    reader.readNextStartElement();
    spiceData.imagePath = reader.readElementText();

    return spiceData;
}

void SpiceInfo::readContentFile(const QString &path)
{
    QFile file(path);
    if(!file.open(QFile::ReadOnly | QFile::Text))
    {
        qDebug() << __FILE__ << __LINE__ << "cannot open file.";
    }

    readContent(&file);
}

void SpiceInfo::readContent(QIODevice *device)
{
    QXmlStreamReader reader(device);

    reader.readNextStartElement();
    if(reader.name() == "SpiceContent")
    {
        while(reader.readNextStartElement())
        {
            if(reader.name() == "Spice")
            {
                int id = reader.attributes().value("id").toInt();

                while(reader.readNextStartElement())
                {
                    if(reader.name() == "Content")
                    {
                        SpiceContent spiceContent;
                        spiceContent.id = reader.attributes().value("id").toString().toInt();
                        spiceContent.retentionTime = reader.attributes().value("retentionTime").toString().toDouble();
                        spiceContent.englishName = reader.attributes().value("englishName").toString().trimmed();
                        spiceContent.chineseName = reader.attributes().value("chineseName").toString().trimmed();
                        spiceContent.absoluteContent = reader.attributes().value("absoluteContent").toString().toDouble();
                        spiceContent.relativeContent = reader.attributes().value("relativeContent").toString().toDouble();
                        m_mapSpiceInfo[id].vecContent.push_back(spiceContent);
                        reader.skipCurrentElement();
                    }
                }
            }
            else
            {
                reader.skipCurrentElement();
            }
        }
    }

}

void SpiceInfo::writeSpiceFile(const QString &path)
{
    QFile file(path);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << __FILE__ << __LINE__ << "cannot open file.";
    }

    writeSpice(&file);
}

void SpiceInfo::writeSpice(QIODevice *device)
{
    QXmlStreamWriter writer(device);

    writer.setAutoFormatting(true);

    writer.writeStartDocument();
    writer.writeStartElement("SpiceInfo");

    auto ite = m_mapSpiceInfo.begin();
    for(; ite != m_mapSpiceInfo.end(); ++ite)
    {
        writeSingleSpice(writer, ite.value());
    }

    writer.writeEndElement();//SpiceInfo
    writer.writeEndDocument();
}

void SpiceInfo::writeSingleSpice(QXmlStreamWriter &writer, const SpiceInfoData &spiceData)
{
    writer.writeStartElement("Spice");
    {
        //id
        writer.writeTextElement("Id", QString::number(spiceData.id));
        //名称
        writer.writeStartElement("Name");
        writer.writeAttribute("englishName", spiceData.name.EnList.join(';'));
        writer.writeAttribute("chineseName", spiceData.name.CnList.join(';'));
        writer.writeEndElement();
        //类型
        writer.writeTextElement("Type", QString::number(spiceData.type));
        //管理状况
        writer.writeStartElement("Management");
        writer.writeAttribute("FEMA", spiceData.management.FEMA);
        writer.writeAttribute("FDA", spiceData.management.FDA);
        writer.writeAttribute("COE", spiceData.management.COE);
        writer.writeAttribute("GB", spiceData.management.GB);
        writer.writeEndElement();
        //性状描述
        writer.writeTextElement("Property", spiceData.property);
        //感官特征
        writer.writeTextElement("Sense", spiceData.sense);
        //物理性质
        writer.writeStartElement("Physics");
        writer.writeAttribute("density", spiceData.physics.density);
        writer.writeAttribute("refractive", spiceData.physics.refractive);
        writer.writeAttribute("solubility", spiceData.physics.solubility);
        writer.writeEndElement();
        //制备提取
        writer.writeTextElement("Extract", spiceData.extract);
        //主要产地
        writer.writeTextElement("Origin", spiceData.origin);
        //作用描述
        writer.writeTextElement("Purpose", spiceData.purpose);
        //气质图谱
        writer.writeTextElement("ImagePath", spiceData.imagePath);
    }
    writer.writeEndElement();//Spice
}

void SpiceInfo::writeContentFile(const QString &path)
{
    QFile file(path);
    if(!file.open(QFile::WriteOnly | QFile::Text))
    {
        qDebug() << __FILE__ << __LINE__ << "cannot open file.";
    }

    writeContent(&file);
}

void SpiceInfo::writeContent(QIODevice *device)
{
    QXmlStreamWriter writer(device);

    writer.setAutoFormatting(true);

    writer.writeStartDocument();
    writer.writeStartElement("SpiceContent");

    auto ite = m_mapSpiceInfo.begin();
    for(; ite != m_mapSpiceInfo.end(); ++ite)
    {
        writer.writeStartElement("Spice");
        writer.writeAttribute("id", QString::number(ite.value().id));
        writeSingleContent(writer, ite.value().vecContent);
        writer.writeEndElement();//Spice
    }

    writer.writeEndElement();//SpiceContent
    writer.writeEndDocument();
}

void SpiceInfo::writeSingleContent(QXmlStreamWriter &writer, const QVector<SpiceContent> &vecContent)
{
    auto ite = vecContent.begin();
    for(; ite != vecContent.end(); ++ite)
    {
        writer.writeStartElement("Content");
        writer.writeAttribute("id", QString::number(ite->id));
        writer.writeAttribute("retentionTime", QString::number(ite->retentionTime));
        writer.writeAttribute("englishName", ite->englishName.trimmed());
        writer.writeAttribute("chineseName", ite->chineseName.trimmed());
        writer.writeAttribute("absoluteContent", QString::number(ite->absoluteContent));
        writer.writeAttribute("relativeContent", QString::number(ite->relativeContent));
        writer.writeEndElement();//Content
    }
}


QMap<int, SpiceInfoData> &SpiceInfo::allSpice()
{
    return m_mapSpiceInfo;
}

//查询香料名称的模糊匹配字段//
QStringList SpiceInfo::queryHazySpice(const QString &name)
{
    QStringList listText;
    bool isChinese = name.contains(QRegExp("[\\x4e00-\\x9fa5]+"));

    auto ite = m_mapSpiceInfo.begin();
    for(; ite != m_mapSpiceInfo.end(); ++ite)
    {
        if(listText.size() > 10)
        {
            break;
        }

        //查找中英文名称//
        const SpiceName &spiceName = ite.value().name;
        if(isChinese)
        {
            //从中文名称中查找//
            for(int i = 0; i < spiceName.CnList.size(); ++i)
            {
                if(spiceName.CnList.at(i).indexOf(name) != -1)
                {
                    listText << spiceName.CnList.at(i);
                }
            }
        }
        else
        {
            //从英文名称中查找//
            for(int i = 0; i < spiceName.EnList.size(); ++i)
            {
                if(spiceName.EnList.at(i).indexOf(name) != -1)
                {
                    listText << spiceName.EnList.at(i);
                }
            }
        }
    }
    return listText;
}

//查询主要成分的模糊匹配字段//
QStringList SpiceInfo::queryHazyContent(const QString &text)
{
    QStringList listText;
    bool isChinese = text.contains(QRegExp("[\\x4e00-\\x9fa5]+"));

    auto ite = m_mapSpiceInfo.begin();
    for(; ite != m_mapSpiceInfo.end(); ++ite)
    {
        if(listText.size() > 10)
        {
            break;
        }

        QVector<SpiceContent> &vecContent = ite->vecContent;
        for(int i = 0; i < vecContent.size(); ++i)
        {
            //从中文名称中查找//
            if(vecContent.at(i).chineseName.indexOf(text) != -1)
            {
                listText << vecContent.at(i).chineseName;
            }

            if(!isChinese)
            {
                //从英文名称中查找//
                if(vecContent.at(i).englishName.indexOf(text) != -1)
                {
                    listText << vecContent.at(i).englishName;
                }
            }
        }
    }
    return listText;
}

void SpiceInfo::save()
{
    QString path = QApplication::applicationDirPath() + "/data/spiceInfo.xml";
    //读取香料
    writeSpiceFile(path);

    path = QApplication::applicationDirPath() + "/data/spiceContent.xml";
    //读取香料成分
    writeContentFile(path);
}
