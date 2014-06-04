#include "vertex.h"
#include "halfedge.h"

using namespace heds;

Vertex::Vertex(): e_edge(NULL)
{
}

Vertex::~Vertex()
{
    e_edge = NULL;
}

const QVector4D& Vertex::geometry()const
{
    return m_geometry;
}

void Vertex::setGeometry(const QVector4D& geometry)
{
    m_geometry = geometry;
}

const HalfEdge * Vertex::edge()const
{
    return e_edge;
}

HalfEdge * Vertex::edge()
{
    return e_edge;
}

void Vertex::setEdge(HalfEdge * edge)
{
    e_edge = edge;
}

Vertex::iterator Vertex::begin()
{
    return iterator(e_edge, e_edge);
}

Vertex::iterator Vertex::end()
{
    return iterator(e_edge, NULL);
}
