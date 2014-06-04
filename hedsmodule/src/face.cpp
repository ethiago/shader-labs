#include "face.h"
#include "halfedge.h"
#include "vertex.h"

using namespace heds;

Face::Face() : e_outer( (HalfEdge *)0 )
{
}
Face::~Face()
{
    e_outer = (HalfEdge *)0;
}

void Face::setOuterEdge(HalfEdge* edge)
{
    e_outer = edge;
}

HalfEdge const * Face::outerEdge()const
{
    return e_outer;
}

HalfEdge *Face::outerEdge()
{
    return e_outer;
}

int Face::numberOfVertices()const
{
    int count = 0;

    HalfEdge *he = e_outer->next();
    while(he != e_outer)
    {
        ++count;
        he = he->next();
    }
    return ++count;
}

Face::iterator Face::begin()const
{
    return iterator(e_outer,e_outer);
}

Face::iterator Face::end() const
{
    return iterator(e_outer,(HalfEdge*)0);
}

QVector3D Face::getNormal()const
{
    QVector3D n;

    if(numberOfVertices() >= 3)
    {
        iterator it = begin();
        QVector3D v1 = (*it)->origin()->geometry().toVector3DAffine();
        ++it;
        QVector3D v2 = (*it)->origin()->geometry().toVector3DAffine();
        ++it;
        QVector3D v3 = (*it)->origin()->geometry().toVector3DAffine();

        n = QVector3D::normal(v1,v2,v3);
    }

    return n;
}
