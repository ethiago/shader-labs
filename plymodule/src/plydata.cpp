#include "plydata.h"

#include <QDebug>


PLYData::PLYData()
{
}

PLYData::PLYData(const PLYData& cp)
{
    *this = cp;
}

PLYData::~PLYData()
{

}

const PLYData& PLYData::operator=(const PLYData& cp)
{
    m_header.clear();
    m_data.clear();

    for(int e = 0; e < cp.getNumberOfElements(); ++e)
    {
        addElement(cp[e]);
    }
    endHeader();

    for(int e = 0; e < cp.getNumberOfElements(); ++e)
    {
        for(unsigned int i = 0; i < cp[e].count(); ++i)
        {
            for(int p = 0; p < cp[e].getNumberOfProperties(); ++p)
            {
                bool ret = setData(e,p,i, cp.getData(e,p,i) );
                Q_ASSERT( ret == true );
            }
        }
    }

    return *this;
}

int PLYData::elementLocation(const QString& name) const
{
    int idx = -1;
    for(int i = 0; i < m_header.size(); ++i)
    {
        if(name == m_header[i].name())
        {
            idx = i;
            break;
        }
    }
    return idx;
}

int PLYData::find(const PLYDataHeader::Element& e)const
{
    int idx = -1;
    for(int i = 0; i < m_header.size(); ++i)
    {
        if(e == m_header[i])
        {
            idx = i;
            break;
        }
    }
    return idx;
}

const PLYDataHeader::Element& PLYData::operator[](int i)const
{
    Q_ASSERT(i >=0 && i < m_header.size());

    return m_header[i];
}

int PLYData::getNumberOfElements()const
{
    return m_header.size();
}

bool PLYData::operator==(const PLYData& cp)const
{
    if(getNumberOfElements() != cp.getNumberOfElements())
        return false;

    for(int me = 0; me < m_header.size(); ++me)
    {
        PLYDataHeader::Element e = m_header[me];
        int te = cp.find(e);
        if(te < 0)
            return false;

        if(e.getNumberOfProperties() != cp[te].getNumberOfProperties())
            return false;

        for(int mp = 0; mp < e.getNumberOfProperties(); ++mp)
        {
            PLYDataHeader::Property p = e[mp];
            int tp = cp[te].find(p);
            if(tp < 0)
                return false;

            for(unsigned int i = 0; i < e.count(); ++i)
            {
                if(getData(me,mp,i) != cp.getData(te,tp,i))
                    return false;
            }
        }
    }

    return true;
}

bool PLYData::operator!=(const PLYData& cp)const
{
    return !(*this == cp);
}

PLYData::HeaderIterator PLYData::beginElement()const
{
    return m_header.begin();
}

PLYData::HeaderIterator PLYData::endElement()const
{
    return m_header.end();
}

int PLYData::addElement(const PLYDataHeader::Element& e)
{
    if(!m_header.contains(e))
    {
        m_header.append(e);
        return m_header.size()-1;
    }

    return -1;
}

int PLYData::getOffset(int elementIdx) const
{
    if( elementIdx < 0 || elementIdx >= m_header.size() )
        return -1;

    int off = 0;
    for(int i = 0; i < elementIdx; ++i)
    {
        off += m_header[i].getNumberOfProperties()*m_header[i].count();
    }
    return off;
}

const QList<QVariant> &PLYData::getData(int elementIdx, int propertyIdx, int itemIdx)const
{
    int off = getOffset(elementIdx);
    Q_ASSERT(off >= 0);

    off += (itemIdx)*m_header[elementIdx].getNumberOfProperties();

    off += propertyIdx;


    return m_data[off];
}

bool PLYData::setData(int elementIdx , int propertyIdx, int itemIdx, const QList<QVariant>& data)
{

    int off = getOffset(elementIdx);
    if(off < 0)
        return false;

    off += (itemIdx)*m_header[elementIdx].getNumberOfProperties();

    off += propertyIdx;

    m_data[off] = data;

    return true;
}

void PLYData::endHeader(void)
{
    int off = 0;
    for(int i = 0; i < m_header.size(); ++i)
    {
        off += m_header[i].getNumberOfProperties()*m_header[i].count();
    }

    QList<QVariant> p;
    m_data.clear();
    for(int i = 0; i < off; ++i)
    {
        m_data.append(p);
    }
}

void PLYData::dumpDebug()const
{
    qDebug() << "m_data";
    qDebug() << m_data;
    qDebug() << " ";

}
