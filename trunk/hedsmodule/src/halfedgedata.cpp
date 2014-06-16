#include "halfedgedata.h"
#include "vertex.h"
#include "halfedge.h"
#include "face.h"

#include <QPair>
#include <QMap>
#include <QDebug>

using namespace heds;

HalfEdgeData::HalfEdgeData():needEnd(true)
{
}

HalfEdgeData::HalfEdgeData(const HalfEdgeData &hed):needEnd(false)
{
    QMap< Vertex*, Vertex* > vmap;
    QMap< Face* , Face* > fmap;
    QMap<HalfEdge*, HalfEdge*> hmap;

    m_uniformValues = hed.m_uniformValues;

    for(int i = 0; i < hed.m_vertices.size(); ++i)
    {
        Vertex * v = new Vertex(*hed.m_vertices[i]);
        vmap.insert(hed.m_vertices[i], v);
        m_vertices.append(v);
    }

    for(int i = 0; i < hed.m_edges.size(); ++i)
    {
        HalfEdge * h = new HalfEdge(*hed.m_edges[i]);
        hmap.insert(hed.m_edges[i],h);
        m_edges.append(h);
    }

    for(int i = 0; i < hed.m_faces.size(); ++i)
    {
        Face * f = new Face(*hed.m_faces[i]);
        fmap.insert(hed.m_faces[i], f);
        m_faces.append(f);
    }

    for(int i = 0; i < m_vertices.size(); ++i)
    {
        HalfEdge * h = hed.m_vertices[i]->edge();
        m_vertices[i]->setEdge( hmap[ h ] );
    }

    for(int i = 0; i < m_faces.size(); ++i)
    {
        HalfEdge * h = hed.m_faces[i]->outerEdge();
        m_faces[i]->setOuterEdge( hmap[ h ] );
    }

    for(int i = 0; i < m_edges.size(); ++i)
    {
        Vertex * v = hed.m_edges[i]->origin();
        m_edges[i]->setOrigin( vmap[ v ] );

        Face * f = hed.m_edges[i]->face();
        if(f != NULL)
            m_edges[i]->setFace( fmap[ f ] );

        HalfEdge * h = hed.m_edges[i]->next();
        m_edges[i]->setNext( hmap[ h ] );

        h = hed.m_edges[i]->twin();
        m_edges[i]->setTwin( hmap[ h ] );
    }

    for(int i = 0; i < hed.e_boundary.size(); ++i)
    {
        e_boundary.append( hmap[ hed.e_boundary[i] ] );
    }
}

bool HalfEdgeData::check()const
{
    for(int i = 0; i < m_vertices.size(); ++i)
    {
        if(m_vertices[i]->edge() == NULL)
        {
            qWarning() << "Vertex Problem " << i;
            return false;
        }
    }

    for(int i = 0; i < m_faces.size(); ++i)
    {
        if(m_faces[i]->outerEdge() == NULL)
        {
            qWarning() << "Face Problem " << i;
            return false;
        }
    }

    for(int i = 0; i < m_edges.size(); ++i)
    {
        if(m_edges[i]->twin() == NULL)
        {
            qWarning() << "Twin Problem " << i;
            return false;
        }
        if(m_edges[i]->twin()->twin() == NULL)
        {
            qWarning() << "Twin Twin NULL " << i;
            return false;
        }
        if(m_edges[i]->twin()->twin() != m_edges[i])
        {
            qWarning() << "Twin Twin does not match" << i;
            return false;
        }
        if(m_edges[i]->next() == NULL)
        {
            qWarning() << "Next Problem " << i;
            return false;
        }
        if(m_edges[i]->origin() == NULL)
        {
            qWarning() << "Origin Problem " << i;
            return false;
        }
        if(m_edges[i]->getDestiny() == NULL)
        {
            qWarning() << "destiny Problem " << i;
            return false;
        }
    }
    return true;
}

void HalfEdgeData::clearVertex()
{
    QList<Vertex*>::iterator it;
    for(it = m_vertices.begin(); it != m_vertices.end(); ++it)
    {
        if( (*it)->edge() == NULL )
        {
            it = m_vertices.erase(it);
            it--;
        }
    }
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

    e_boundary.clear();
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

int HalfEdgeData::numberOfBondaries()const
{
    return e_boundary.size();
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

const HalfEdge* HalfEdgeData::getBoundaryReferenceEdge(int i)
{
    if(i >= 0 && i < e_boundary.size())
        return e_boundary[i];

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

int HalfEdgeData::addFace(const QList<int>& vertexIndices)
{
    static QMap<QPair<Vertex*, Vertex*>, HalfEdge*> edges;
    QMap<QPair<Vertex*, Vertex*>, HalfEdge*>::iterator it;

    Face *face = NULL;

    if(vertexIndices.size() < 3)
    {
        qWarning() << "Face with " << vertexIndices.size() << " indices";
        return -1;
    }

    face = new Face();
    m_faces.append(face);

    HalfEdge *first = new HalfEdge();
    m_edges.append(first);
    face->setOuterEdge(first);
    first->setFace(face);
    Vertex * v = getVertex(vertexIndices[0]);
    first->setOrigin( v );
    v->setEdge(first);


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
        m_edges.append(he);
        he->setFace(face);
        Vertex *v = getVertex(vertexIndices[i]);
        he->setOrigin( v );
        v->setEdge(he);


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

    return m_faces.size()-1;

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
    if(!he->hasTwin() || he->twin()->face() == NULL)
        return he;
    he = he->twin();


    while( he != first  )
    {
        he = he->next();
        if( !he->hasTwin() || he->twin()->face() == NULL)
            return he;
        he = he->twin();
    }

    return NULL;
}

void HalfEdgeData::endMesh()
{
    if(!needEnd)
        return;

    clearVertex();

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
        m_edges.append(twinF);
        e_boundary.append(twinF);
        twinF->setFace( NULL );
        twinF->setTwin( nullTwin );
        nullTwin->setTwin( twinF );
        twinF->setOrigin(nullTwin->getDestiny());

        HalfEdge * prev = twinF;
        HalfEdge * next = nextNull(nullTwin);
        while( next != nullTwin )
        {
            HalfEdge *twin = new HalfEdge();
            m_edges.append(twin);
            twin->setFace( NULL );
            twin->setTwin( next );
            next->setTwin( twin );
            twin->setOrigin( next->getDestiny() );

            twin->setNext(prev);

            prev = twin;
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

const QList<QVariant>& HalfEdgeData::uniformValue(int uniformId)const
{
    Q_ASSERT(uniformId >=0 && uniformId < m_uniformValues.size());
    return m_uniformValues[uniformId];
}

void HalfEdgeData::setUniform(int uniformId, const QList<QVariant>& data)
{
    Q_ASSERT(uniformId >=0);

    if( uniformId >= m_uniformValues.size())
    {
        QList<QVariant> t;
        for(int j = 0; j <= uniformId-m_uniformValues.size() ; ++j)
            m_uniformValues.append(t);
    }
    m_uniformValues[uniformId] = data;
}
