#include "SpiceInfo.h"

#include <QFile>
#include <QXmlStreamReader>
#include <QXmlStreamWriter>
#include <QMessageBox>
#include <QApplication>
#include <QSqlQuery>
#include <QSqlDatabase>
#include <QDebug>
#include <QSqlError>

#include "ConnectionPool.h"

#define USE_DB

#ifdef USE_DB
SpiceInfo::SpiceInfo(QObject *parent)
    :QObject(parent)
{
}

SpiceInfo::~SpiceInfo()
{

}

void SpiceInfo::appendSpice(const SpiceInfoData &spiceInfo)
{
    QSqlDatabase db;
    if(!createConnection(db))
    {
        ConnectionPool::closeConnection(db);
        return;
    }

    //spice_information//
    int informationId = -1;
    QSqlQuery query(db);
    query.prepare("INSERT INTO spice_information (englishName, chineseName, typeId, property, sense, extract, origin, purpose, imagePath, FEMA, FDA, COE, GB, density, refractive, solubility)"
                  "VALUES (:englishName, :chineseName, :typeId, :property, :sense, :extract, :origin, :purpose, :imagePath, :FEMA, :FDA, :COE, :GB, :density, :refractive, :solubility)");

    query.bindValue(":englishName", spiceInfo.name.EnList.join(';'));
    query.bindValue(":chineseName", spiceInfo.name.CnList.join(';'));
    query.bindValue(":typeId", spiceInfo.type);
    query.bindValue(":property", spiceInfo.property);
    query.bindValue(":sense", spiceInfo.sense);
    query.bindValue(":extract", spiceInfo.extract);
    query.bindValue(":origin", spiceInfo.origin);
    query.bindValue(":purpose", spiceInfo.purpose);
    query.bindValue(":imagePath", spiceInfo.imagePath);
    query.bindValue(":FEMA", spiceInfo.management.FEMA);
    query.bindValue(":FDA", spiceInfo.management.FDA);
    query.bindValue(":COE", spiceInfo.management.COE);
    query.bindValue(":GB", spiceInfo.management.GB);
    query.bindValue(":density", spiceInfo.physics.density);
    query.bindValue(":refractive", spiceInfo.physics.refractive);
    query.bindValue(":solubility", spiceInfo.physics.solubility);

    if(query.exec())
    {
        query.prepare("SELECT id FROM spice_information WHERE chineseName=:chineseName");
        query.bindValue(":chineseName", spiceInfo.name.CnList.join(';'));
        if(query.exec())
        {
            if(query.next())
            {
                informationId = query.value(0).toInt();
            }
        }
        else
        {
            qDebug() << __FILE__ << __LINE__ << query.lastError().text();
        }
    }
    else
    {
        qDebug() << __FILE__ << __LINE__ << query.lastError().text();
    }


    if(informationId != -1)
    {
        QSqlQuery query(db);
        for(int i = 0; i < spiceInfo.vecContent.size(); ++i)
        {
            //content//
            int contentId = getContentId(db, spiceInfo.vecContent.at(i));
            if(contentId == -1)
                continue;
            //spice_content//
            query.prepare("INSERT INTO spice_content (retentionTime, absoluteValue, relativeValue, spiceId, contentId)"
                          " VALUES (:retentionTime, :absoluteValue, :relativeValue, :spiceId, :contentId)");
            query.bindValue(":retentionTime", spiceInfo.vecContent.at(i).retentionTime);
            query.bindValue(":absoluteValue", spiceInfo.vecContent.at(i).absoluteContent);
            query.bindValue(":relativeValue", spiceInfo.vecContent.at(i).relativeContent);
            query.bindValue(":spiceId", informationId);
            query.bindValue(":contentId", contentId);
            if(!query.exec())
            {
                qDebug() << __FILE__ << __LINE__ << query.lastError().text();
            }
        }
    }
    // 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);

}

void SpiceInfo::deleteSpice(int id)
{
    QSqlDatabase db;
    if(!createConnection(db))
    {
        // 连接使用完后需要释放回数据库连接池
        ConnectionPool::closeConnection(db);
        return;
    }

    //spice_content//
    {
        QSqlQuery query(db);
        query.prepare("DELETE FROM spice_content WHERE spiceId=:id");
        query.bindValue(":id", id);
        if(!query.exec())
        {
            qDebug() << __FILE__ << __LINE__ << query.lastError().text();
        }
    }

    //spice_information//
    {
        QSqlQuery query(db);
        query.prepare("DELETE FROM spice_information WHERE id=:id");
        query.bindValue(":id", id);
        if(!query.exec())
        {
            qDebug() << __FILE__ << __LINE__ << query.lastError().text();
        }
    }

    // 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);
}

void SpiceInfo::modifySpice(SpiceInfoData &spiceInfo)
{
    QSqlDatabase db;
    if(!createConnection(db))
    {
        // 连接使用完后需要释放回数据库连接池
        ConnectionPool::closeConnection(db);
        return;
    }
    //spice_information//
    QSqlQuery query(db);

    //spice_information//
    query.prepare("UPDATE spice_information SET englishName=:englishName,chineseName=:chineseName,typeId=:typeId,property=:property,sense=:sense,extract=:extract,origin=:origin,purpose=:purpose,imagePath=:imagePath,FEMA=:FEMA,FDA=:FDA,COE=:COE,GB=:GB,density=:density,refractive=:refractive,solubility=:solubility WHERE id=:id");
    query.bindValue(":englishName", spiceInfo.name.EnList.join(';'));
    query.bindValue(":chineseName", spiceInfo.name.CnList.join(';'));
    query.bindValue(":typeId", spiceInfo.type);
    query.bindValue(":property", spiceInfo.property);
    query.bindValue(":sense", spiceInfo.sense);
    query.bindValue(":extract", spiceInfo.extract);
    query.bindValue(":origin", spiceInfo.origin);
    query.bindValue(":purpose", spiceInfo.purpose);
    query.bindValue(":imagePath", spiceInfo.imagePath);
    query.bindValue(":FEMA", spiceInfo.management.FEMA);
    query.bindValue(":FDA", spiceInfo.management.FDA);
    query.bindValue(":COE", spiceInfo.management.COE);
    query.bindValue(":GB", spiceInfo.management.GB);
    query.bindValue(":density", spiceInfo.physics.density);
    query.bindValue(":refractive", spiceInfo.physics.refractive);
    query.bindValue(":solubility", spiceInfo.physics.solubility);
    query.bindValue(":id", spiceInfo.id);

    if(!query.exec())
    {
        qDebug() << __FILE__ << __LINE__ << query.lastError().text();
    }

    //spice_content
    //先删除香料的成分数据//
    query.prepare("DELETE FROM spice_content WHERE spiceId=:id");
    query.bindValue(":id", spiceInfo.id);
    if(!query.exec())
    {
        qDebug() << __FILE__ << __LINE__ << query.lastError().text();
    }

    for(int i = 0; i < spiceInfo.vecContent.size(); ++i)
    {
        //content//
        int contentId = getContentId(db, spiceInfo.vecContent.at(i));
        if(contentId == -1)
            continue;
        //spice_content//
        query.prepare("INSERT INTO spice_content (retentionTime, absoluteValue, relativeValue, spiceId, contentId) "
                      "VALUES (:retentionTime, :absoluteValue, :relativeValue, :spiceId, :contentId)");

        query.bindValue(":retentionTime", spiceInfo.vecContent.at(i).retentionTime);
        query.bindValue(":absoluteValue", spiceInfo.vecContent.at(i).absoluteContent);
        query.bindValue(":relativeValue", spiceInfo.vecContent.at(i).relativeContent);
        query.bindValue(":spiceId", spiceInfo.id);
        query.bindValue(":contentId", contentId);
        if(!query.exec())
        {
            qDebug() << __FILE__ << __LINE__ << query.lastError().text();
        }
    }


    // 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);
}

bool SpiceInfo::findSpice(int id, SpiceInfoData &spiceInfo)
{
    QSqlDatabase db;
    if(!createConnection(db))
    {
        return false;
    }

    QSqlQuery query(db);
    //spice_information
    query.prepare("SELECT englishName, chineseName, typeId, property, sense, extract, origin, purpose, imagePath, FEMA, FDA, COE, GB, density, refractive, solubility FROM spice_information WHERE id=:id");
    query.bindValue(":id", id);
    if(!query.exec())
    {
        qDebug() << __FILE__ << __LINE__ << query.lastError().text();
    }

    if(query.next())
    {
        spiceInfo.id = id;
        spiceInfo.name.EnList = query.value(0).toString().split(';');
        spiceInfo.name.CnList = query.value(1).toString().split(';');
        spiceInfo.type = query.value(2).toInt();
        spiceInfo.property = query.value(3).toString();
        spiceInfo.sense = query.value(4).toString();
        spiceInfo.extract = query.value(5).toString();
        spiceInfo.origin = query.value(6).toString();
        spiceInfo.purpose = query.value(7).toString();
        spiceInfo.imagePath = query.value(8).toString();
        spiceInfo.management.FEMA = query.value(9).toString();
        spiceInfo.management.FDA = query.value(10).toString();
        spiceInfo.management.COE = query.value(11).toString();
        spiceInfo.management.GB = query.value(12).toString();
        spiceInfo.physics.density = query.value(13).toString();
        spiceInfo.physics.refractive = query.value(14).toString();
        spiceInfo.physics.solubility = query.value(15).toString();

        //spice_content
        query.prepare("SELECT retentionTime, absoluteValue, relativeValue, contentId FROM spice_content WHERE spiceId=:id");
        query.bindValue(":id", id);
        if(!query.exec())
        {
            qDebug() << __FILE__ << __LINE__ << query.lastError().text();
        }

        SpiceContent content;
        while(query.next())
        {
            content.retentionTime = query.value(0).toDouble();
            content.absoluteContent = query.value(1).toDouble();
            content.relativeContent = query.value(2).toDouble();
            int contentId = query.value(3).toInt();
            QSqlQuery query_content(db);
            query_content.prepare("SELECT chineseName, englishName FROM content WHERE id=:id");
            query_content.bindValue(":id", contentId);
            if(!query_content.exec())
            {
                qDebug() << __FILE__ << __LINE__ << query.lastError().text();
            }

            if(!query_content.next())
            {
                Q_ASSERT(false);
                continue;
            }
            content.chineseName = query_content.value(0).toString();
            content.englishName = query_content.value(1).toString();

            spiceInfo.vecContent.push_back(content);
        }
    }

    // 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);

    return true;
}

QVector<SpiceInfoData> SpiceInfo::querySpice(const QString &name)
{
    QVector<SpiceInfoData> vecSpiceInfo;

    QSqlDatabase db;
    if(!createConnection(db))
    {
        // 连接使用完后需要释放回数据库连接池
        ConnectionPool::closeConnection(db);
        return vecSpiceInfo;
    }

    QSqlQuery query(db);
    //spice_information
    query.prepare("SELECT id, englishName, chineseName, typeId, property, sense, extract, origin, purpose, imagePath, FEMA, FDA, COE, GB, density, refractive, solubility FROM spice_information "
                  "WHERE chineseName like :chineseName OR englishName like :englishName");
    query.bindValue(":chineseName", "%" + name + "%");
    query.bindValue(":englishName", "%" + name + "%");
    if(!query.exec())
    {
        qDebug() << __FILE__ << __LINE__ << query.lastError().text();
    }

    while(query.next())
    {

        SpiceInfoData spiceInfo;
        spiceInfo.id = query.value(0).toInt();
        spiceInfo.name.EnList = query.value(1).toString().split(';');
        spiceInfo.name.CnList = query.value(2).toString().split(';');
        spiceInfo.type = query.value(3).toInt();
        spiceInfo.property = query.value(4).toString();
        spiceInfo.sense = query.value(5).toString();
        spiceInfo.extract = query.value(6).toString();
        spiceInfo.origin = query.value(7).toString();
        spiceInfo.purpose = query.value(8).toString();
        spiceInfo.imagePath = query.value(9).toString();
        spiceInfo.management.FEMA = query.value(10).toString();
        spiceInfo.management.FDA = query.value(11).toString();
        spiceInfo.management.COE = query.value(12).toString();
        spiceInfo.management.GB = query.value(13).toString();
        spiceInfo.physics.density = query.value(14).toString();
        spiceInfo.physics.refractive = query.value(15).toString();
        spiceInfo.physics.solubility = query.value(16).toString();

        //spice_content
        QSqlQuery query_content(db);
        query_content.prepare("SELECT retentionTime, absoluteValue, relativeValue, contentId FROM spice_content "
                              "WHERE spiceId=:id");
        query_content.bindValue(":id", spiceInfo.id);
        if(!query_content.exec())
        {
            qDebug() << __FILE__ << __LINE__ << query_content.lastError().text();
        }

        SpiceContent content;
        while(query_content.next())
        {
            content.retentionTime = query_content.value(0).toDouble();
            content.absoluteContent = query_content.value(1).toDouble();
            content.relativeContent = query_content.value(2).toDouble();
            int contentId = query_content.value(3).toInt();
            QSqlQuery query_1(db);
            query_1.prepare("SELECT chineseName, englishName FROM content WHERE id=:id");
            query_1.bindValue(":id",contentId);
            if(query_1.exec())
            {
                if(query_1.next())
                {
                    content.chineseName = query_1.value(0).toString();
                    content.englishName = query_1.value(1).toString();

                    spiceInfo.vecContent.push_back(content);
                }
            }
            else
            {
                qDebug() << __FILE__ << __LINE__ << query_1.lastError().text();
            }
        }
        vecSpiceInfo.push_back(spiceInfo);
    }
    // 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);

    return vecSpiceInfo;
}

QVector<SpiceByContent> SpiceInfo::queryContent(const QString &name)
{
    QVector<SpiceByContent> vecSpice;

    QSqlDatabase db;
    if(!createConnection(db))
    {
        return vecSpice;
    }

    QSqlQuery query(db);
    //content
    query.prepare("SELECT id, englishName, chineseName FROM content "
                  "WHERE chineseName like :chineseName OR englishName like :englishName");
    query.bindValue(":chineseName", "%" + name + "%");
    query.bindValue(":englishName", "%" + name + "%");
    if(!query.exec())
    {
        qDebug() << __FILE__ << __LINE__ << query.lastError().text();
    }
    SpiceByContent content;
    while(query.next())
    {
        int contentId = query.value(0).toInt();
        QSqlQuery query_content(db);
        query_content.prepare("SELECT spiceId, relativeValue, absoluteValue FROM spice_content "
                              "WHERE contentId=:id");
        query_content.bindValue(":id", contentId);
        if(query_content.exec())
        {
            while(query_content.next())
            {
                content.id = query_content.value(0).toInt();
                content.relativeContent = query_content.value(1).toDouble();
                content.absoluteContent = query_content.value(2).toDouble();

                QSqlQuery query_information(db);
                query_information.prepare("SELECT englishName, chineseName FROM spice_information "
                                          "WHERE id=:id");
                query_information.bindValue(":id", content.id);
                if(query_information.exec())
                {
                    if(query_information.next())
                    {
                        content.name.EnList = query_information.value(0).toString().split(';');
                        content.name.CnList = query_information.value(1).toString().split(';');

                        vecSpice.push_back(content);
                    }
                }
                else
                {
                    qDebug() << __FILE__ << __LINE__ << query_information.lastError().text();
                }
            }
        }
        else
        {
            qDebug() << __FILE__ << __LINE__ << query_content.lastError().text();
        }
    }
    // 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);
    return vecSpice;
}

QMap<int, QString> SpiceInfo::allSpice(int type)
{
    QMap<int, QString> mapSpice;
    QSqlDatabase db;
    if(!createConnection(db))
    {
        return mapSpice;
    }

    QSqlQuery query(db);
    //spice_information
    query.prepare("SELECT id, chineseName FROM spice_information WHERE typeId=:typeId");
    query.bindValue(":typeId", type);
    if(query.exec())
    {
        while(query.next())
        {
            mapSpice[query.value(0).toInt()] = query.value(1).toString().split(';').at(0);
        }
    }
    else
    {
        qDebug() << __FILE__ << __LINE__ << query.lastError().text();
    }

    // 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);
    return mapSpice;
}

//查询香料名称的模糊匹配字段//
QStringList SpiceInfo::queryHazySpice(const QString &name)
{
    QStringList listText;

    QSqlDatabase db;
    if(!createConnection(db))
    {
        // 连接使用完后需要释放回数据库连接池
        ConnectionPool::closeConnection(db);
        return listText;
    }

    bool isChinese = name.contains(QRegExp("[\\x4e00-\\x9fa5]+"));

    QSqlQuery query(db);
    QString text;
    //spice_information
    if(!isChinese)
    {
        query.prepare("SELECT englishName FROM spice_information WHERE englishName like :englishName");
        query.bindValue(":englishName","%" + name + "%");
        if(!query.exec())
        {
            qDebug() << __FILE__ << __LINE__ << query.lastError().text();
        }

        while(query.next())
        {
            text = query.value(0).toString();
            if(!text.isEmpty())
            {
                QStringList list = text.split(';');
                for(int i = 0; i < list.size(); ++i)
                {
                    if(list.at(i).contains(name))
                    {
                        listText << list.at(i);
                    }
                }
            }
        }
    }

    query.prepare("SELECT chineseName FROM spice_information WHERE chineseName like :chineseName");
    query.bindValue(":chineseName", "%" + name + "%");
    if(!query.exec())
    {
        qDebug() << __FILE__ << __LINE__ << query.lastError().text();
    }

    while(query.next())
    {
        text = query.value(0).toString();
        if(!text.isEmpty())
        {
            QStringList list = text.split(';');
            for(int i = 0; i < list.size(); ++i)
            {
                if(list.at(i).contains(name))
                {
                    listText << list.at(i);
                }
            }
        }
    }

    // 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);

    return listText;
}

//查询主要成分的模糊匹配字段//
QStringList SpiceInfo::queryHazyContent(const QString &name)
{
    QStringList listText;

    QSqlDatabase db;
    if(!createConnection(db))
    {
        // 连接使用完后需要释放回数据库连接池
        ConnectionPool::closeConnection(db);
        return listText;
    }

    bool isChinese = name.contains(QRegExp("[\\x4e00-\\x9fa5]+"));

    QSqlQuery query(db);
    QString text;
    //content
    if(!isChinese)
    {
        query.prepare("SELECT englishName FROM content WHERE englishName like :englishName");
        query.bindValue(":englishName","%" + name + "%");
        if(!query.exec())
        {
            qDebug() << __FILE__ << __LINE__ << query.lastError().text();
        }

        while(query.next())
        {
            text = query.value(0).toString();
            if(!text.isEmpty())
            {
                listText << text;
            }
        }
    }

    query.prepare("SELECT chineseName FROM content WHERE chineseName like :chineseName");
    query.bindValue(":chineseName","%" + name + "%");
    if(!query.exec())
    {
        qDebug() << __FILE__ << __LINE__ << query.lastError().text();
    }

    while(query.next())
    {
        text = query.value(0).toString();
        if(!text.isEmpty())
        {
            listText << text;
        }
    }
    // 连接使用完后需要释放回数据库连接池
    ConnectionPool::closeConnection(db);
    return listText;
}

bool SpiceInfo::createConnection(QSqlDatabase &db)
{
    // 从数据库连接池里取得连接
    db = ConnectionPool::openConnection();

    if(!db.isValid())
    {
        QMessageBox::critical(0, qApp->tr("Cannot open database"),
                              qApp->tr("Unable to establish a database connection.\n"
                                       "This example needs SQLite support. Please read "
                                       "the Qt SQL driver documentation for information how "
                                       "to build it.\n\n"
                                       "Click Cancel to exit."), QMessageBox::Cancel);
        return false;
    }

    return true;
}

int SpiceInfo::getContentId(QSqlDatabase &db, const SpiceContent &content)
{
    QSqlQuery query(db);

    query.prepare("SELECT id FROM content WHERE englishName=:englishName AND chineseName=:chineseName");
    query.bindValue(":englishName", content.englishName);
    query.bindValue(":chineseName", content.chineseName);
    if(!query.exec())
    {
        //数据插入失败
        qDebug() << __FILE__ << __LINE__ << query.lastError().text();
    }

    if(query.next())
    {
        return query.value(0).toInt();
    }

    //数据不存在//
    query.prepare("INSERT INTO content (englishName, chineseName) VALUES (:englishName, :chineseName)");
    query.bindValue(":englishName", content.englishName);
    query.bindValue(":chineseName", content.chineseName);
    if(!query.exec())
    {
        //数据插入失败
        qDebug() << __FILE__ << __LINE__ << query.lastError().text();
        return -1;
    }

    query.prepare("SELECT id FROM content WHERE englishName=:englishName AND chineseName=:chineseName");
    query.bindValue(":englishName", content.englishName);
    query.bindValue(":chineseName", content.chineseName);
    if(!query.exec())
    {
        //数据插入失败
        qDebug() << __FILE__ << __LINE__ << query.lastError().text();
    }

    if(query.next())
    {
        return query.value(0).toInt();
    }

    return -1;
}

#else
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
#endif
