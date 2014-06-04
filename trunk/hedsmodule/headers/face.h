#ifndef FACE_H
#define FACE_H

#include "halfedge.h"

namespace heds{

class Face
{
    HalfEdge * e_outer;

public:
    Face();
    ~Face();

    void setOuterEdge(HalfEdge* edge);

    HalfEdge const *  outerEdge()const;
    HalfEdge *outerEdge();

    int numberOfVertices()const;

    class iterator{
        HalfEdge * e_first;
        HalfEdge * base;
    public:
        iterator(HalfEdge * f, HalfEdge * b){ e_first = f; base = b; }
        iterator(const iterator & it) { e_first = it.e_first; base = it.base; }

        const HalfEdge * operator*(){ return base; }

        bool operator==(const iterator& it){ return (e_first == it.e_first && base == it.base); }
        bool operator!=(const iterator& it){ return (e_first != it.e_first || base != it.base); }

        iterator & operator=(const iterator & it) { e_first = it.e_first; base = it.base; return *this; }

        iterator & operator++(){ base = base->next(); if(base==e_first) base = (HalfEdge*)0; return *this; }
    };

    iterator begin();
    iterator end();
};

}

#endif // FACE_H
