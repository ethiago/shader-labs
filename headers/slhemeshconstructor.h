#ifndef SLHEMESHCONSTRUCTOR_H
#define SLHEMESHCONSTRUCTOR_H

#include "meshcontainer.h"
#include "halfedgedata.h"
#include "Object3D.h"
#include "slhemesh.h"

class SLHEMeshConstructor : public MeshContainer
{
    heds::HalfEdgeData * e_mesh;

    SLHEMesh * e_obj;

public:
    SLHEMeshConstructor();

    int addVertex(float x, float y, float z);
    int addAttribute(const QString& name, QVariant::Type type, bool isList = false);
    void setAttribute(int vertexId, int attributeId, const QList<QVariant>& data);

    int addFace(const QList<int>& vertexIndices);
    int addFaceUniform(const QString& name, QVariant::Type type, bool isList = false);
    void setFaceUniform(int faceId, int uniformId, const QList<QVariant>& data );

    int addUniform(const QString& name, QVariant::Type type, bool isList = false);
    void setUniform(int uniformId, const QList<QVariant>& data );

    SLHEMesh * getObject();
};

#endif // SLHEMESHCONSTRUCTOR_H
