#pragma once

#include <QString>

#include "Common.h"

class QIODevice;
class QXmlStreamReader;

class XMLReader
{
public:
    XMLReader();
    ~XMLReader();

    bool read(QIODevice *device);

    QString errorString() const;

private:
    void readSpice();

private:
    QXmlStreamReader *m_pXmlReader;
};

