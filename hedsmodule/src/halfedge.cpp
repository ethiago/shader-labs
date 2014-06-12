#include "halfedge.h"
#include "vertex.h"

using namespace heds;

HalfEdge::HalfEdge() : e_origin(NULL), e_next(NULL), e_twin(NULL), e_face(NULL)
{
}

HalfEdge::HalfEdge(const HalfEdge& h) : e_origin(NULL), e_next(NULL), e_twin(NULL), e_face(NULL)
{

}

HalfEdge::~HalfEdge()
{
    e_origin = NULL;
    e_next = NULL;
    e_twin = NULL;
    e_face = NULL;
}

void HalfEdge::setOrigin(Vertex * v)
{
    e_origin = v;
}
void HalfEdge::setNext(HalfEdge * he)
{
    e_next = he;
}
void HalfEdge::setTwin(HalfEdge * he)
{
    e_twin = he;
}
void HalfEdge::setFace(Face * face)
{
    e_face = face;
}

Vertex const * HalfEdge::origin()const
{
    return e_origin;
}
Vertex * HalfEdge::origin()
{
    return e_origin;
}

HalfEdge const * HalfEdge::next()const
{
    return e_next;
}
HalfEdge * HalfEdge::next()
{
    return e_next;
}

HalfEdge const * HalfEdge::twin()const
{
    return e_twin;
}
HalfEdge * HalfEdge::twin()
{
    return e_twin;
}

bool HalfEdge::hasTwin()const
{
    return (e_twin != NULL);
}

Face const * HalfEdge::face()const
{
    return e_face;
}
Face * HalfEdge::face()
{
    return e_face;
}

Vertex const * HalfEdge::getDestiny()const
{
    if(e_next == NULL)
        return NULL;

    return e_next->next()->origin();

}
Vertex * HalfEdge::getDestiny()
{
    if(e_next == NULL)
        return NULL;

    return e_next->next()->origin();
}
