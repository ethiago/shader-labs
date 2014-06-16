#ifndef VERTEX_H
#define VERTEX_H

#include <QVector4D>
#include <QVector3D>
#include <QVector2D>

#include "halfedge.h"

namespace heds
{

class Vertex
{
    QVector4D m_geometry;

    QVector3D m_normal;

    QVector2D m_texCoord;

    HalfEdge * e_edge;

    QList<QList<QVariant> > m_attribValues;

public:
    Vertex();
    Vertex(const Vertex& v);
    ~Vertex();

    const QVector4D& geometry()const;
    void setGeometry(const QVector4D& geometry);

    const QVector3D& normal()const;
    void setNormal(const QVector3D& normal);

    const QVector2D& texCoord()const;
    void setTexCoord(const QVector2D& texCoord);

    const HalfEdge * edge()const;
    HalfEdge * edge();
    void setEdge(HalfEdge * edge);

    const QList<QVariant>& attribValue(int i)const;
    void setAttribValue(int i, const QList<QVariant>& value);

    class iterator{
        HalfEdge * e_first;
        HalfEdge * base;
    public:
        iterator():e_first(NULL), base(NULL){  }
        iterator(HalfEdge * f, HalfEdge * b) : e_first(f), base(b) {}
        iterator(const iterator & it) { e_first = it.e_first; base = it.base; }
        const HalfEdge * operator*()const{ return base; }

        bool operator==(const iterator& it){ return (e_first == it.e_first && base == it.base); }
        bool operator!=(const iterator& it){ return (e_first != it.e_first || base != it.base); }

        iterator & operator=(const iterator & it) { e_first = it.e_first; base = it.base; return *this; }

        iterator & operator++(){ base = base->twin()->next();
                                 if(base==e_first) base = (HalfEdge*)0;
                                 return *this; }
    };

    iterator begin() const;
    iterator end() const;
};

}

#endif // VERTEX_H
