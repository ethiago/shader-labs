#ifndef PLYIO_H
#define PLYIO_H

#include <QTextStream>

#include "plydata.h"
#include "plydataitens.h"

class PLYIO
{
    PLYData m_data;
    QTextStream m_s;

public:
    PLYIO();
    ~PLYIO();

    bool load(const QString& fn);
    bool write(const QString& fn);

    const PLYData& getData()const;
    void setData(const PLYData&);

private:

    enum KeyWord{
        PLY,
        Format,
        Element,
        Property,
        EndHeader,
        NoKey
    };

    KeyWord nextKey();
    QString nextData();

    bool validateFormat();
    bool validateHeaderBeginer();

    bool readElement(PLYDataHeader::Element *e);
    bool readProperty(PLYDataHeader::Property *p);
    bool readData();
    bool readVariant(PLYDataHeader::Property::Type t, QVariant *v);

    static PLYDataHeader::Property::Type convertType(const QString& type);

    bool writeData(QTextStream& s)const;
    void writeVariantType(QTextStream& s, PLYDataHeader::Property::Type t)const;
    void writeVariant(QTextStream& s, PLYDataHeader::Property::Type t, const QVariant &v)const;
    
};

#endif // PLYIO_H
