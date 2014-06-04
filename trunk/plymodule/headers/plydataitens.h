#ifndef PLYDATAITENS_H
#define PLYDATAITENS_H

#include <QString>
#include <QList>
#include <QVariant>

namespace PLYDataHeader{

class Property
{
public:
    enum Type{
        CHAR = QVariant::Char,
        INT = QVariant::Int,
        FLOAT = QVariant::Double
    };
private:

    QString m_name;
    Type m_type;
    bool m_list;

public:


    Property();
    Property(const QString& n, Type t, bool list = false);
    Property(const Property& cp);
    ~Property();

    const QString& name()const;
    void name(const QString& n);

    Type type()const;
    void type(Type t);

    bool isList()const;
    void setList(bool l);

    const Property& operator=(const Property& cp);
    bool operator==(const Property& cp)const;
    bool operator!=(const Property& cp)const;

};

class Element
{
    QString m_name;
    unsigned int m_count;
    QList<Property> m_properties;

public:
    Element();
    Element(const QString& n);
    Element(const Element& e);
    ~Element();

    const QString& name()const;
    void name(const QString& n);

    unsigned int count()const;
    void count(unsigned int c);
    int getNumberOfProperties()const;

    int addProperty(const Property& );
    int addProperty(const QString& n, Property::Type t, bool list = false);

    typedef QList<Property>::ConstIterator PropertiesIterator;
    PropertiesIterator beginProperties()const;
    PropertiesIterator endProperties()const;

    int propertyLocation(const QString&name)const;
    int find(const Property& p)const;

    const Property& operator[](int i)const;
    //Property& operator[](int i);

    const Element& operator=(const Element& e);
    bool operator==(const Element& e)const;
    bool operator!=(const Element& e)const;
};

}

#endif // PLYDATAITENS_H
