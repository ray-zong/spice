#pragma once

#include <QString>
#include <QVector>
#include <QStringList>

class SpiceContent
{
public:
    int id;                  //id
    double retentionTime;    //保留时间
    QString englishName;     //英文名称
    QString chineseName;     //中文名称
    double relativeContent;  //相对含量
    double absoluteContent;  //绝对含量
};

class Spice
{
public:
    QString chineseName;
    QVector<SpiceContent> vecContent;
};

//香料名称
struct SpiceName
{
    QStringList EnList;   //英文名称
    QStringList CnList;   //中文名称
};

//管理状况
struct Management
{
    QString FEMA;
    QString FDA;
    QString COE;
    QString GB;
};

//物理性质
struct Physics
{
    QString density;    //相对密度
    QString refractive; //折光率
    QString solubility; //溶解性
};

//香料结构定义
struct SpiceInfoData
{
    int id;                //id
    SpiceName name;        //名称
    int type;              //类型
    Management management; //管理状况
    QString property;      //性状描述
    QString sense;         //感官特征
    Physics physics;       //物理性质
    QString extract;       //制备提取
    QString origin;        //主要产地
    QString purpose;       //作用描述
    QString imagePath;     //气质图谱
    QVector<SpiceContent> vecContent;

    SpiceInfoData():id(-1){}
};

//通过成分查找的香料结果
struct SpiceByContent
{
    int id;                 //id
    SpiceName name;         //名称
    double relativeContent; //相对含量
    double absoluteContent; //绝对含量
};

//香料类型定义
enum SpiceType
{
    Oils = 0,
    Tinctures = 1,
    Concretes = 2,
    Others = 3
};

//用户类型//
enum UserType
{
    Administrator = 0,
    OrdinaryUser = 1
};

//用户//
struct User
{
    QString user;
    QString password;
    UserType type;
};
#define SAFE_DELETE(x) if(x){delete x; x = NULL;}

