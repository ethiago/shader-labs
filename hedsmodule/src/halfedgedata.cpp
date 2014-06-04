#include "halfedgedata.h"
#include "vertex.h"
#include "halfedge.h"
#include "face.h"

#include <QPair>
#include <QMap>

using namespace heds;

HalfEdgeData::HalfEdgeData() : m_ext(new Face())
{
}
HalfEdgeData::~HalfEdgeData()
{
    for(int i = 0; i < m_vertices.size(); ++i)
    {
        delete m_vertices[i];
    }
    m_vertices.clear();

    for(int i = 0; i < m_edges.size(); ++i)
    {
        delete m_edges[i];
    }
    m_edges.clear();

    for(int i = 0; i < m_faces.size(); ++i)
    {
        delete m_faces[i];
    }
    m_faces.clear();

    delete m_ext;
}

int HalfEdgeData::numberOfVertices()const
{
    return m_vertices.size();
}
int HalfEdgeData::numberOfHalfEdges()const
{
    return m_edges.size();
}
int HalfEdgeData::numberOfFaces()const
{
    return m_faces.size();
}

Vertex* HalfEdgeData::getVertex(int i)
{
    if(i >= 0 && i < m_vertices.size())
    {
        return m_vertices[i];
    }
    return NULL;
}

const Vertex* HalfEdgeData::getVertex(int i)const
{
    if(i >= 0 && i < m_vertices.size())
    {
        return m_vertices[i];
    }
    return NULL;
}

int HalfEdgeData::addVertex()
{
    m_vertices.append(new Vertex());
    return m_vertices.size()-1;
}

Face* HalfEdgeData::getFace(int i)
{
    if(i >= 0 && i < m_faces.size())
    {
        return m_faces[i];
    }
    return NULL;
}

const Face* HalfEdgeData::getFace(int i)const
{
    if(i >= 0 && i < m_faces.size())
    {
        return m_faces[i];
    }
    return NULL;
}

Face* HalfEdgeData::addFace(const QList<int>& vertexIndices)
{
    static QMap<QPair<Vertex*, Vertex*>, HalfEdge*> edges;
    QMap<QPair<Vertex*, Vertex*>, HalfEdge*>::iterator it;

    Face *ret = NULL;

    if(vertexIndices.size() < 3)
        return ret;

    ret = new Face();

    HalfEdge *first = new HalfEdge();
    ret->setOuterEdge(first);
    first->setFace(ret);
    first->setOrigin( getVertex(vertexIndices[0]) );


    it = edges.find(qMakePair( getVertex(vertexIndices[1]),
                          getVertex(vertexIndices[0])));
    if( it != edges.end() )
    {
        HalfEdge *twin = (*it);
        first->setTwin(twin);
        twin->setTwin(first);
        edges.erase(it);
    }else
    {
        edges.insert(qMakePair( getVertex(vertexIndices[0]),
                     getVertex(vertexIndices[1])), first);
    }
    HalfEdge *prev = first;

    for(int i = 1; i < vertexIndices.size(); ++i)
    {
        int iN = (i+1)%vertexIndices.size();
        HalfEdge *he = new HalfEdge();
        he->setFace(ret);
        he->setOrigin( getVertex(vertexIndices[i]) );

        it = edges.find(qMakePair( getVertex(vertexIndices[iN]),
                              getVertex(vertexIndices[i])));
        if( it != edges.end() )
        {
            HalfEdge *twin = (*it);
            he->setTwin(twin);
            twin->setTwin(he);
            edges.erase(it);
        }else
        {
            edges.insert(qMakePair( getVertex(vertexIndices[i]),
                         getVertex(vertexIndices[iN])), he);
        }

        prev->setNext(he);
        prev = he;
    }

    prev->setNext(first);

    return ret;

}

HalfEdge* HalfEdgeData::getHalfEdge(int i)
{
    if( i>= 0 && i < m_edges.size())
        return m_edges[i];

    return NULL;
}

const HalfEdge* HalfEdgeData::getHalfEdge(int i)const
{
    if( i>= 0 && i < m_edges.size())
        return m_edges[i];

    return NULL;
}

HalfEdge * HalfEdgeData::nextNull(HalfEdge * first)
{
    HalfEdge *he = first->next();
    if(!he->hasTwin())
        return he;
    he = he->twin();

    while( he != first  )
    {
        he = he->next();
        if(!he->hasTwin())
            return he;
        he = he->twin();
    }

    return NULL;
}

void HalfEdgeData::endMesh()
{
    HalfEdge * nullTwin = NULL;
    for(int i = 0; i < m_edges.size(); ++i)
    {
        if(!m_edges[i]->hasTwin())
        {
            nullTwin = m_edges[i];
            break;
        }
    }

    while(nullTwin != NULL)
    {
        HalfEdge *twinF = new HalfEdge();
        twinF->setOrigin( nullTwin->getDestiny() );
        twinF->setFace( m_ext );
        twinF->setTwin( nullTwin );
        nullTwin->setTwin( twinF );

        HalfEdge * prev = twinF;
        HalfEdge * next = nextNull(nullTwin);
        while( next != nullTwin )
        {
            HalfEdge *twin = new HalfEdge();
            twin->setOrigin( next->getDestiny() );
            twin->setFace( m_ext );
            twin->setTwin( next );
            next->setTwin( twinF );

            next->setNext(prev);
            prev = next;
            next = nextNull(next);
        }
        twinF->setNext(prev);

        nullTwin = NULL;
        for(int i = 0; i < m_edges.size(); ++i)
        {
            if(!m_edges[i]->hasTwin())
            {
                nullTwin = m_edges[i];
                break;
            }
        }
    }
}
