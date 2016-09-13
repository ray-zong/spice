#pragma once

#include <QObject>
#include <QMap>

#include "Common.h"

class QIODevice;
class QXmlStreamReader;
class QXmlStreamWriter;

#include <QSqlDatabase>

#define USE_DB

#ifdef USE_DB
class SpiceInfo : public QObject
{
    Q_OBJECT

public:
    explicit SpiceInfo(QObject *parent = 0);
    ~SpiceInfo();

    //添加数据
    void appendSpice(const SpiceInfoData &);
    //删除数据:名称
    void deleteSpice(int);
    //修改数据:名称
    void modifySpice(SpiceInfoData &);
    //查找数据：id
    bool findSpice(int id, SpiceInfoData &spiceInfo);
    //查找数据:名称
    QVector<SpiceInfoData> querySpice(const QString &name);
    //查找数据：主要成分
    QVector<SpiceByContent> queryContent(const QString &name);
    //获取香料(类型)
    QMap<int, QString> allSpice(int type);
    //查询香料名称的模糊匹配字段//
    QStringList queryHazySpice(const QString &name);
    //查询主要成分的模糊匹配字段//
    QStringList queryHazyContent(const QString &name);

protected:

private:
    bool createConnection(QSqlDatabase &db);

    //获取content的id//
    int getContentId(QSqlDatabase &db, const SpiceContent &content);

    //查找spice_management的数据
    bool getManagementById(QSqlDatabase &db, Management &management, int id);

    //查找spice_management的数据
    bool getPhysicsById(QSqlDatabase &db, Physics &physics, int id);
};
#else
class SpiceInfo : public QObject
{
    Q_OBJECT

public:
    explicit SpiceInfo(QObject *parent = 0);
    ~SpiceInfo();

    //添加数据
    void appendSpice(const SpiceInfoData &);
    //删除数据:名称
    void deleteSpice(int);
    //修改数据:名称
    void updateSpice(SpiceInfoData &);
    //查找数据：id
    bool findSpice(int id, SpiceInfoData &spiceInfo);
    //查找数据:名称
    QVector<SpiceInfoData> querySpice(const QString &name);
    //查找数据：主要成分
    QVector<SpiceByContent> queryContent(const QString &);
    //获取所有香料
    QMap<int, SpiceInfoData> &allSpice();
    //查询香料名称的模糊匹配字段//
    QStringList queryHazySpice(const QString &);
    //查询主要成分的模糊匹配字段//
    QStringList queryHazyContent(const QString &);

protected:

private:
    //保存//
    void save();

    //读取香料文件//
    void readSpiceFile(const QString &path);
    //读取香料//
    void readSpice(QIODevice *);
    //读取单一香料//
    SpiceInfoData readSingleSpice(QXmlStreamReader &);

    //读取成分文件//
    void readContentFile(const QString &path);
    //读取成分//
    void readContent(QIODevice *);
    //读取单一成分//
    SpiceContent readSingleContent(QXmlStreamReader &);


    //写入XML文件
    void writeSpiceFile(const QString &path);
    //写入香料
    void writeSpice(QIODevice *);
    //写入单一香料
    void writeSingleSpice(QXmlStreamWriter &, const SpiceInfoData &);

    //写入成分文件//
    void writeContentFile(const QString &path);
    //写入香料成分//
    void writeContent(QIODevice *);
    //写入单一成分//
    void writeSingleContent(QXmlStreamWriter &, const QVector<SpiceContent> &);

private:
    QMap<int, SpiceInfoData> m_mapSpiceInfo;
};

#endif
