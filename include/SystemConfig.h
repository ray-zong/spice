#pragma once

#include <QString>

class SystemConfig
{
public:
    SystemConfig();
    ~SystemConfig();

    //当前主要成分的显示量
    inline void setMainContentCount(int count){ m_nMainContentCount = count; }
    inline int getMainContentCount() const{ return m_nMainContentCount; }

    //气质图谱文件位置
    inline void setImageFilePath(const QString &path){ m_strImageFilePath = path; }
    inline QString getImageFilePath() const{ return m_strImageFilePath; }

private:
    void readConfigFile(const QString &path);

    void writeConfigFile(const QString &path);

private:
    int m_nMainContentCount;
    QString m_strImageFilePath;
};
