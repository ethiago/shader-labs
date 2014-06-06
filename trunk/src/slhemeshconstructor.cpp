#include "slhemeshconstructor.h"
#include "vertex.h"
#include "face.h"
#include "plydataitens.h"

#include <QVector4D>

using namespace heds;

SLHEMeshConstructor::SLHEMeshConstructor() : e_mesh(new heds::HalfEdgeData()), e_obj(new SLHEMesh)
{
}

int SLHEMeshConstructor::addVertex(float x, float y, float z)
{
    int iV = e_mesh->addVertex();
    Vertex *v = e_mesh->getVertex(iV);
    v->setGeometry(QVector4D(x,y,z,1.0));

    return iV;
}

int SLHEMeshConstructor::addAttribute(const QString& name, QVariant::Type type, bool isList)
{
    PLYDataHeader::Property p;
    p.name(name);
    p.type((PLYDataHeader::Property::Type) type);
    p.setList(isList);

    return e_obj->addAttribute(p);
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

int SLHEMeshConstructor::addFaceUniform(const QString& name, QVariant::Type type, bool isList)
{
    PLYDataHeader::Property p;
    p.name(name);
    p.type((PLYDataHeader::Property::Type) type);
    p.setList(isList);

    return e_obj->addFaceUniform(p);
}

void SLHEMeshConstructor::setFaceUniform(int faceId, int uniformId, const QList<QVariant>& data )
{

}

int SLHEMeshConstructor::addUniform(const QString& name, QVariant::Type type, bool isList)
{
    PLYDataHeader::Property p;
    p.name(name);
    p.type((PLYDataHeader::Property::Type) type);
    p.setList(isList);

    return e_obj->addUniform(p);
}

void SLHEMeshConstructor::setUniform(int uniformId, const QList<QVariant>& data )
{

}

SLHEMesh * SLHEMeshConstructor::getObject()
{
    e_mesh->endMesh();
    e_obj->setData(e_mesh);
    return e_obj;
}
