#include "plydataitens.h"


using namespace PLYDataHeader;

Property::Property() :
    m_name(QString()), m_type(FLOAT), m_list(false)
{
}

Property::Property(const QString& n, Type t, bool list) :
    m_name(n), m_type(t), m_list(list)
{
}

Property::Property(const Property& cp) :
    m_name(cp.name()), m_type(cp.type()), m_list(cp.isList())
{
}

Property::~Property()
{
}

const QString& Property::name()const
{
    return m_name;
}
void Property::name(const QString& n)
{
    m_name = n;
}

Property::Type Property::type()const
{
    return m_type;
}

void Property::type(Property::Type t)
{
    m_type = t;
}

bool Property::isList()const
{
    return m_list;
}

void Property::setList(bool l)
{
    m_list = l;
}

const Property& Property::operator=(const Property& cp)
{
    m_name = cp.name();
    m_type = cp.type();
    m_list = cp.isList();
    return *this;
}

bool Property::operator==(const Property& cp)const
{
    return (m_name == cp.name() && m_type == cp.type() && m_list == cp.isList());
}

bool Property::operator!=(const Property& cp)const
{
    return !(*this == cp);
}

//**********************************************//

Element::Element(): m_name(QString()), m_count(0)
{
}

Element::Element(const QString& n): m_name(n), m_count(0)
{

}

Element::Element(const Element& e)
{
    *this = e;
}

Element::~Element()
{

}

const QString& Element::name()const
{
    return m_name;
}
void Element::name(const QString& n)
{
    m_name = n;
}

unsigned int Element::count()const
{
    return m_count;
}

void Element::count(unsigned int c)
{
    m_count = c;
}

int Element::getNumberOfProperties()const
{
    return m_properties.size();
}

int Element::addProperty(const Property& p)
{
    if(!m_properties.contains(p))
    {
        m_properties.append(p);
        return m_properties.size()-1;
    }

    return -1;
}

int Element::addProperty(const QString& n, Property::Type t, bool list)
{
    return addProperty( Property(n,t, list));
}

int Element::propertyLocation(const QString& name)const
{
    int idx = -1;
    for(int i = 0; i < m_properties.size(); ++i)
    {
        if(name == m_properties[i].name())
        {
            idx = i;
            break;
        }
    }

    return idx;
}

int Element::find(const Property& p)const
{
    int idx = -1;
    for(int i = 0; i < m_properties.size(); ++i)
    {
        if(p == m_properties[i])
        {
            idx = i;
            break;
        }
    }

    return idx;
}

const Property& Element::operator[](int i)const
{
    Q_ASSERT(i >= 0 && i < m_properties.size());

    return m_properties[i];
}

//Property& Element::operator[](int i)
//{
//    Q_ASSERT(i >= 0 && i < m_properties.size());

//    return m_properties[i];
//}

Element::PropertiesIterator Element::beginProperties()const
{
    return m_properties.begin();
}

Element::PropertiesIterator Element::endProperties()const
{
    return m_properties.end();
}

const Element& Element::operator=(const Element& e)
{
    m_name = e.name();
    m_count = e.count();
    m_properties.clear();

    QList<Property>::ConstIterator it;
    for(it = e.beginProperties(); it != e.endProperties(); ++it)
    {
        addProperty(*it);
    }

    return *this;
}

bool Element::operator==(const Element& e)const
{
    if(m_name != e.name())
        return false;

    if(m_count != e.count())
        return false;

    QList<Property>::ConstIterator it;
    int n = 0;
    for(it = e.beginProperties(); it != e.endProperties(); ++it)
    {
        if(!m_properties.contains(*it))
            return false;
        ++n;
    }

    return n == m_properties.size();
}

bool Element::operator!=(const Element& e)const
{
    return !(*this == e);
}
