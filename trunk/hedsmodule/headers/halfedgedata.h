#ifndef HALFEDGEDATA_H
#define HALFEDGEDATA_H

#include <QList>
#include <QMap>
#include <QPair>
#include <QVariant>
#include <QVariant>

namespace heds{

class Vertex;
class HalfEdge;
class Face;

class HalfEdgeData
{
    QList<Vertex*> m_vertices;
    QList<HalfEdge*> m_edges;
    QList<Face*> m_faces;

    QList<HalfEdge*> e_boundary;

    QList< QList< QVariant > > m_uniformValues;

    const bool needEnd;

public:
    HalfEdgeData();
    HalfEdgeData(const HalfEdgeData& hed);
    ~HalfEdgeData();

    int numberOfVertices()const;
    int numberOfHalfEdges()const;
    int numberOfFaces()const;

    int numberOfBondaries()const;

    int addVertex();
    Vertex* getVertex(int i);
    const Vertex* getVertex(int i)const;

    int addFace(const QList<int>& vertexIndices);
    Face* getFace(int i);
    const Face* getFace(int i)const;

    const HalfEdge* getBoundaryReferenceEdge(int i);

    HalfEdge* getHalfEdge(int i);
    const HalfEdge* getHalfEdge(int i)const;

    void endMesh();

    const QList<QVariant>& uniformValue(int uniformId)const;
    void setUniform(int uniformId, const QList<QVariant>& data);

    bool check()const;
    void clearVertex();

private:
    HalfEdge * nextNull(HalfEdge *);

};

}

#endif // HALFEDGEDATA_H
