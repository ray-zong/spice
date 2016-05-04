#pragma once

#include "Common.h"

class QIODevice;
class QXmlStreamWriter;

class XMLWriter
{
public:
    XMLWriter();
    ~XMLWriter();

    bool write(QIODevice *device, const QVector<Spice>&);

private:
    QXmlStreamWriter *m_pXmlWriter;
};
