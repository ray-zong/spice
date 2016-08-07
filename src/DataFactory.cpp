#include "DataFactory.h"

#include <QApplication>
#include <QString>
#include <QFile>
#include <QDebug>

#include "SpiceInfo.h"
#include "SystemConfig.h"

DataFactory *DataFactory::m_pDataFactory = NULL;

DataFactory::~DataFactory()
{
    SAFE_DELETE(m_pSpiceInfo);
    SAFE_DELETE(m_pSystemConfig);
}

DataFactory *DataFactory::instance()
{
    if(m_pDataFactory == NULL)
    {
        m_pDataFactory = new DataFactory;
    }
    return m_pDataFactory;
}

void DataFactory::disinstance()
{
    SAFE_DELETE(m_pDataFactory);
}

SpiceInfo *DataFactory::getSpiceInfo()
{
    if(m_pSpiceInfo == NULL)
    {
        m_pSpiceInfo = new SpiceInfo;
    }

    return m_pSpiceInfo;
}

SystemConfig *DataFactory::getSystemConfig()
{
    if(m_pSystemConfig == NULL)
    {
        m_pSystemConfig = new SystemConfig;
    }
    return m_pSystemConfig;
}

DataFactory::DataFactory()
    : m_pSpiceInfo(NULL)
    , m_pSystemConfig(NULL)
{

}

