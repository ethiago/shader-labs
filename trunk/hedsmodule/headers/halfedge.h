#ifndef HALFEDGE_H
#define HALFEDGE_H

namespace heds
{

class Vertex;
class Face;

class HalfEdge
{
    Vertex * e_origin;
    HalfEdge * e_next;
    HalfEdge * e_twin;

    Face * e_face;

public:
    HalfEdge();
    ~HalfEdge();

    void setOrigin(Vertex * v);
    void setNext(HalfEdge * he);
    void setTwin(HalfEdge * he);
    void setFace(Face * face);

    Vertex const * origin()const;
    Vertex * origin();

    Vertex const * getDestiny()const;
    Vertex * getDestiny();

    HalfEdge const * next()const;
    HalfEdge * next();

    HalfEdge const * twin()const;
    HalfEdge * twin();

    Face const * face()const;
    Face * face();


    bool hasTwin()const;

};

}

#endif // HALFEDGE_H
