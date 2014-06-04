#ifndef SLHEMESH_H
#define SLHEMESH_H

#include "Object3D.h"
#include "halfedgedata.h"


class SLHEMesh : public Object3D
{

    heds::HalfEdgeData * m_data;

public:
    SLHEMesh(heds::HalfEdgeData * data);
    ~SLHEMesh();
};

#endif // SLHEMESH_H
