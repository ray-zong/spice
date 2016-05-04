#pragma once

#include <QVector>
#include <QStringList>

#include "Common.h"

class DataFactory
{
public:
    static DataFactory *instance();
    static void disinstance();

    const QVector<Spice> &getSpices();
    //
    void addSpice(const Spice &);
    void removeSpice(const QString &spiceName);
    void removeSpice(const int &index);
    //
    QStringList queryHazyText(const QString &, int);

    //
    QVector<SpiceContent> queryContentBySpiceName(const QString &spiceName);
    QVector<QStringList> querySpiceByContent(const QString &content);

private:
    DataFactory();
    void init();
    QStringList queryHazySpice(const QString &);
    QStringList queryHazyContent(const QString &);

private:
    static DataFactory *m_pDataFactory;
    QVector<Spice> m_vecSpice;
};

