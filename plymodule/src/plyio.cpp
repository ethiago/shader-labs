#include <QFile>
#include <QTextStream>
#include <QDebug>
#include <QStringList>

#include "plyio.h"

void debugVariant(PLYDataHeader::Property::Type t, const QVariant& v)
{
    switch (t) {
    case PLYDataHeader::Property::CHAR:
        qDebug() << v.value<char>();
        break;
    case PLYDataHeader::Property::INT:
        qDebug() << v.value<int>();
        break;
    case PLYDataHeader::Property::FLOAT:
        qDebug() << v.value<float>();
        break;
    }
}

PLYIO::PLYIO()
{
}

PLYIO::~PLYIO()
{

}

void PLYIO::setData(const PLYData& data)
{
    m_data = data;
}

const PLYData& PLYIO::getData()const
{
    return m_data;
}

bool PLYIO::readProperty(PLYDataHeader::Property *p)
{
    *p = PLYDataHeader::Property();
    QStringList list = m_s.readLine().split(" ", QString::SkipEmptyParts, Qt::CaseInsensitive);
    if(list.size() < 2)
        return false;

    if(list[0].toLower() == "list")
    {
        if(list.size() != 4)
            return false;

        p->type( PLYIO::convertType(list[2].toLower() ) );
        p->name(list[3].toLower());
        p->setList(true);

    }else
    {
        if(list.size() != 2)
            return false;

        p->type(PLYIO::convertType(list[0].toLower() ));
        p->name(list[1].toLower());
        p->setList(false);
    }

    return true;
}

PLYDataHeader::Property::Type PLYIO::convertType(const QString& type)
{
    // char uchar
    if(type == "char" || type == "uchar")
        return PLYDataHeader::Property::CHAR;

    // short ushort int uint int8 uint8 int16 uint16 int32 uint32
    if(type == "short" || type == "ushort" || type == "int" || type == "uint" ||
            type == "int8" || type == "uint8" || type == "int16" ||
            type == "uint16" || type == "int32" || type == "uint32")
        return PLYDataHeader::Property::INT;

    // float double float32 float64
    //if(type == "float" || type == "double" || type == "float32" || type == "float64")
        return PLYDataHeader::Property::FLOAT;


}

bool PLYIO::readElement(PLYDataHeader::Element *e)
{
    *e = PLYDataHeader::Element();

    QStringList list = m_s.readLine().split(" ", QString::SkipEmptyParts, Qt::CaseInsensitive);
    if(list.size() != 2)
        return false;

    bool ok;
    int c = list[1].toInt(&ok);
    if(!ok)
        return false;
    e->count(c);
    e->name(list[0]);

    return true;
}

bool PLYIO::validateFormat()
{
    QString t, v;
    m_s >> t >> v;
    if(t+v != "ascii1.0")
        return false;
    else
        return true;
}

bool PLYIO::validateHeaderBeginer()
{
    KeyWord k = nextKey();

    if(k != PLY)
        return false;

    k=nextKey();

    if(k != Format)
        return false;

    if(!validateFormat())
        return false;

    return true;
}

bool PLYIO::load(const QString& fn)
{
    QFile f(fn);
    if(!f.open(QIODevice::ReadOnly))
        return false;

    QString all = f.readAll();
    m_s.setString(&all);
    f.close();

    PLYDataHeader::Element e;
    PLYDataHeader::Property p;

    if(!validateHeaderBeginer())
        return false;

    int state = 0;
    KeyWord k = nextKey();
    while(k != EndHeader)
    {
        bool readed;
        switch (k) {
        case Element:
            if(state == 1)
                return false;
            if(state == 2)
                m_data.addElement(e);
            state = 1;
            readed = readElement(&e);
            if(!readed)
                return false;
            break;
        case Property:
            if(state == 0)
                return false;
            state = 2;
            readed = readProperty(&p);
            if(!readed)
                return false;
            e.addProperty(p);
            break;
        default:
            return false;
        }

        k = nextKey();
    }

    if(state == 1)
        return false;

    if(state == 2)
    {
        m_data.addElement(e);
    }

    m_data.endHeader();

    return readData();
}

QString PLYIO::nextData()
{
    QString w;

    if(m_s.atEnd())
        return QString();

    m_s >> w;

    if(w == "{" )
    {
        while(w != "}" && !m_s.atEnd())
        {
            m_s >> w;
        }
        if(w == "}")
            return nextData();
    }

    return w;
}

PLYIO::KeyWord PLYIO::nextKey()
{
    QString w;
    m_s >> w;

    while(!m_s.atEnd() && w.isEmpty())  { m_s >> w; }

    if(m_s.atEnd())
        return NoKey;

    w = w.toLower();

    if(w == "obj_info" || w == "comment")
    {
        m_s.readLine();
        return nextKey();
    }


    if(w == "ply")
        return PLY;
    if(w == "format")
        return Format;
    if(w == "element")
        return Element;
    if(w == "property")
        return Property;
    if(w == "end_header")
        return EndHeader;

    return NoKey;
}

bool PLYIO::readData()
{
    PLYData::HeaderIterator it;
    PLYDataHeader::Element::PropertiesIterator jt;
    QList<QVariant> list;

    for(int e = 0; e < m_data.getNumberOfElements(); ++e)
    {
        for(unsigned int i = 0; i < m_data[e].count(); ++i)
        {
            for(int p = 0; p < m_data[e].getNumberOfProperties(); ++p)
            {
                list.clear();
                int t = 1;

                if( m_data[e][p].isList() )
                {

                    QString w = nextData();
                    if(w.isEmpty())
                        return false;

                    bool v;
                    t = w.toInt(&v);
                    if(!v)
                        return false;
                }

                for(int l = 0; l < t; ++l)
                {
                    QVariant v;

                    if(!readVariant(m_data[e][p].type(), &v))
                        return false;

                    list.append(v);
                }

                if(!m_data.setData(e,p, i, list))
                    return false;
            }
        }
    }

    return true;
}

bool PLYIO::readVariant(PLYDataHeader::Property::Type t, QVariant* v)
{
    bool b;

    QString w = nextData();
    if(w.isEmpty())
        return false;

    switch (t) {
    case PLYDataHeader::Property::CHAR:
        *v = QVariant(w[0]);
        break;
    case PLYDataHeader::Property::INT:
        *v = QVariant(w.toInt(&b));
        if(!b)
        {
            qDebug() << "Int Conversion: " << w;
            return false;
        }
        break;
    case PLYDataHeader::Property::FLOAT:
        *v = QVariant(w.toFloat(&b));
        if(!b)
        {
            qDebug() << "Float Conversion: " << w;
            return false;
        }
        break;
    }

    return true;
}

void PLYIO::writeVariant(QTextStream& s, PLYDataHeader::Property::Type t, const QVariant& v)const
{
    switch (t) {
    case PLYDataHeader::Property::CHAR:
        s << v.value<char>();
        break;
    case PLYDataHeader::Property::INT:
        s << v.value<int>();
        break;
    case PLYDataHeader::Property::FLOAT:
        s << v.value<float>();
        break;
    }
}

void PLYIO::writeVariantType(QTextStream& s, PLYDataHeader::Property::Type t)const
{
    switch (t) {
    case PLYDataHeader::Property::CHAR:
        s << "char";
        break;
    case PLYDataHeader::Property::INT:
        s << "int";
        break;
    case PLYDataHeader::Property::FLOAT:
        s << "float";
        break;
    }
}

bool PLYIO::writeData(QTextStream& s)const
{
    QList<QVariant> list;

    for(int e = 0; e < m_data.getNumberOfElements(); ++e)
    {
        for(unsigned int i = 0; i < m_data[e].count(); ++i)
        {
            for(int p = 0; p < m_data[e].getNumberOfProperties(); ++p)
            {
                list = m_data.getData(e,p,i);

                if(m_data[e][p].isList())
                {
                    s << list.size() << " ";
                    for(int l = 0; l < list.size(); ++l)
                    {
                        writeVariant(s, m_data[e][p].type(), list[l] );
                        s << " ";
                    }
                }else
                {
                    writeVariant(s, m_data[e][p].type(), list[0] );
                    s << " ";
                }
            }
            s << endl;
        }
    }

    return true;
}

bool PLYIO::write(const QString& fn)
{
    QFile f(fn);
    if(!f.open(QIODevice::WriteOnly))
        return false;

    QTextStream s(&f);

    s << "ply" << endl;
    s << "format ascii 1.0" << endl;

    PLYData::HeaderIterator it;
    PLYDataHeader::Element::PropertiesIterator jt;

    for(it = m_data.beginElement(); it != m_data.endElement(); ++it)
    {
        s << "element " << (*it).name() << " " << (*it).count() << endl;
        for(jt = (*it).beginProperties(); jt != (*it).endProperties(); ++jt)
        {
            s << "property ";
            if((*jt).isList())
                s << "list uchar ";

            writeVariantType( s, (*jt).type() );

            s << " " << (*jt).name() << endl;
        }
    }
    s << "end_header" << endl;


    return writeData(s);
}
