#ifndef XMLWRITER_H
#define XMLWRITER_H

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

#endif // XMLWRITER_H
