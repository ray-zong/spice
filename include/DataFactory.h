#pragma once

#include <QVector>
#include <QStringList>

#include "Common.h"

class SpiceInfo;
class SystemConfig;

class DataFactory
{
public:
    ~DataFactory();

    static DataFactory *instance();
    static void disinstance();


    //香料信息//
    SpiceInfo *getSpiceInfo();

    //配置信息//
    SystemConfig *getSystemConfig();
private:
    DataFactory();

private:
    static DataFactory *m_pDataFactory;           //数据工厂指针//

    SpiceInfo *m_pSpiceInfo;                      //香料数据类//
    SystemConfig *m_pSystemConfig;                //配置信息//
};

