#include "slhemeshconstructor.h"
#include "vertex.h"
#include "face.h"

#include <QVector4D>

using namespace heds;

SLHEMeshConstructor::SLHEMeshConstructor() : e_mesh(new heds::HalfEdgeData())
{
}

int SLHEMeshConstructor::addVertex(float x, float y, float z)
{
    int iV = e_mesh->addVertex();
    Vertex *v = e_mesh->getVertex(iV);
    v->setGeometry(QVector4D(x,y,z,1.0));

    return iV;
}

int SLHEMeshConstructor::addAttribute(const QString& name, QVariant::Type type)
{
    return e_mesh->addAttribute(name, type);
}

void SLHEMeshConstructor::setAttribute(int vertexId, int attributeId, const QList<QVariant>& data)
{
    Vertex *v = e_mesh->getVertex(vertexId);
    v->setAttribValue(attributeId, data);
}

int SLHEMeshConstructor::addFace(const QList<int>& vertexIndices)
{
    return e_mesh->addFace(vertexIndices);
}

int SLHEMeshConstructor::addFaceUniform(const QString& name, QVariant::Type type)
{
    return 0;
}

void SLHEMeshConstructor::setFaceUniform(int faceId, int uniformId, const QList<QVariant>& data )
{

}

int SLHEMeshConstructor::addUniform(const QString& name, QVariant::Type type)
{
    return 0;
}

void SLHEMeshConstructor::setUniform(int uniformId, const QList<QVariant>& data )
{

}

heds::HalfEdgeData * SLHEMeshConstructor::getMesh()
{
    e_mesh->endMesh();
    return e_mesh;
}
