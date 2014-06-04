#ifndef HALFEDGEDATA_H
#define HALFEDGEDATA_H

#include <QList>
#include <QMap>
#include <QPair>

namespace heds{

class Vertex;
class HalfEdge;
class Face;

class HalfEdgeData
{
    QList<Vertex*> m_vertices;
    QList<HalfEdge*> m_edges;
    QList<Face*> m_faces;

    Face * m_ext;
    QList<HalfEdge*> e_boundary;

public:
    HalfEdgeData();
    ~HalfEdgeData();

    int numberOfVertices()const;
    int numberOfHalfEdges()const;
    int numberOfFaces()const;

    int addVertex();
    Vertex* getVertex(int i);
    const Vertex* getVertex(int i)const;

    Face* addFace(const QList<int>& vertexIndices);
    Face* getFace(int i);
    const Face* getFace(int i)const;

    HalfEdge* getHalfEdge(int i);
    const HalfEdge* getHalfEdge(int i)const;

    void endMesh();

private:
    HalfEdge * nextNull(HalfEdge *);
};

}

#endif // HALFEDGEDATA_H
