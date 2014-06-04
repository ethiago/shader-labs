#include "face.h"
#include "halfedge.h"

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

Face::iterator Face::begin()
{
    return iterator(e_outer,e_outer);
}

Face::iterator Face::end()
{
    return iterator(e_outer,(HalfEdge*)0);
}
