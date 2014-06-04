#ifndef PLYDATA_H
#define PLYDATA_H

#include <QVariant>

#include "plydataitens.h"


class PLYData
{
    QList<PLYDataHeader::Element> m_header;
    QList<QList<QVariant> > m_data;

    int getOffset(int elementIdx)const;

public:
    PLYData();
    PLYData(const PLYData& cp);
    ~PLYData();

    const PLYData& operator=(const PLYData& cp);

    bool operator==(const PLYData& cp)const;
    bool operator!=(const PLYData& cp)const;

    int elementLocation(const QString& name)const;
    int find(const PLYDataHeader::Element& e) const;
    const PLYDataHeader::Element& operator[](int i)const;

    int addElement(const PLYDataHeader::Element& e);

    int getNumberOfElements()const;

    void endHeader(void);

    typedef QList<PLYDataHeader::Element>::ConstIterator HeaderIterator;

    HeaderIterator beginElement()const;
    HeaderIterator endElement()const;

    const QList<QVariant>& getData(int elementIdx , int propertyIdx, int itemIdx)const;
    bool setData(int elementIdx , int propertyIdx, int itemIdx, const QList<QVariant>& data);

    void dumpDebug()const;
};

#endif // PLYDATA_H
