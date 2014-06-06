#ifndef SLHEMESH_H
#define SLHEMESH_H

#include "Object3D.h"
#include "halfedgedata.h"


class SLHEMesh : public Object3D
{
    int indexList;

    heds::HalfEdgeData * m_data;
    const bool myData;

    void drawGeometry(void) const;

public:
    SLHEMesh(heds::HalfEdgeData * data = NULL, bool mydata = true);

    SLHEMesh(const SLHEMesh& cp);
    ~SLHEMesh();

    Object3D* copy(void) const;

    void storeList();

    void setData(heds::HalfEdgeData * data);
};

#endif // SLHEMESH_H
