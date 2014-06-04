#include "slhemesh.h"

SLHEMesh::SLHEMesh(heds::HalfEdgeData * data) : m_data(data)
{
}

SLHEMesh::~SLHEMesh()
{
    delete m_data;
}
