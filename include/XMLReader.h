#ifndef XMLREADER_H
#define XMLREADER_H

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

#endif // XMLREADER_H
