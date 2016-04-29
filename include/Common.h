#ifndef COMMON_H
#define COMMON_H

#include <QString>
#include <QVector>

class SpiceContent
{
public:
    int id;
    double retentionTime;
    QString englishName;
    QString chineseName;
    double value;
    double content;
};

class Spice
{
public:
    QString chineseName;
    QVector<SpiceContent> vecContent;
};

#define SAFE_DELETE(x) if(x){delete x; x = NULL;}

#endif // COMMON_H
