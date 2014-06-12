#ifndef SLHEMESH_H
#define SLHEMESH_H

#include "Object3D.h"
#include "halfedgedata.h"


class SLHEMesh : public Object3D
{
    const QString m_fileName;

    int indexList;

    heds::HalfEdgeData m_data;

    void drawGeometry(void) const;

public:
    SLHEMesh(const QString& fileName);

    SLHEMesh(const SLHEMesh& cp);
    ~SLHEMesh();

    const QString& fileName()const;

    Object3D* copy(void) const;

    void storeList();

    void afterLink(unsigned int programId);

    heds::HalfEdgeData * data();
};

#endif // SLHEMESH_H
