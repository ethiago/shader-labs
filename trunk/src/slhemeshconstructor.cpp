#include "slhemeshconstructor.h"
#include "vertex.h"
#include "face.h"
#include "plydataitens.h"

#include <QVector4D>

using namespace heds;

SLHEMeshConstructor::SLHEMeshConstructor(const QString &fileName) :
    e_obj(new SLHEMesh(fileName))
{
}

int SLHEMeshConstructor::addVertex(float x, float y, float z)
{
    int iV = e_obj->data()->addVertex();
    Vertex *v = e_obj->data()->getVertex(iV);
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
    Vertex *v = e_obj->data()->getVertex(vertexId);
    v->setAttribValue(attributeId, data);
}

void SLHEMeshConstructor::setNormal(int vertexId, float nx, float ny, float nz)
{
    Vertex *v = e_obj->data()->getVertex(vertexId);
    v->setNormal(QVector3D(nx,ny,nz));
}

void SLHEMeshConstructor::setTexCoord(int vertexId, float u, float v)
{
    Vertex *vertex = e_obj->data()->getVertex(vertexId);
    vertex->setTexCoord(QVector2D(u,v));
}

int SLHEMeshConstructor::addFace(const QList<int>& vertexIndices)
{
    return e_obj->data()->addFace(vertexIndices);
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
    Face * f = e_obj->data()->getFace(faceId);
    f->setUniform(uniformId, data);
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
    e_obj->data()->setUniform(uniformId, data);
}

SLHEMesh * SLHEMeshConstructor::getObject()
{
    e_obj->data()->endMesh();
    return e_obj;
}
