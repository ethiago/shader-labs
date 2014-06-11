#ifndef MESHCONTAINER_H
#define MESHCONTAINER_H

#include <QString>
#include <QVariant>
#include <QList>

class MeshContainer
{
public:
    virtual ~MeshContainer(){}

    virtual int addVertex(float x, float y, float z) = 0;

    virtual void setNormal(int vertexId, float nx, float ny, float nz) = 0;
    virtual void setTexCoord(int vertexId, float u, float v) = 0;

    virtual int addAttribute(const QString& name, QVariant::Type type, bool isList = false) = 0;
    virtual void setAttribute(int vertexId, int attributeId, const QList<QVariant>& data) = 0;

    virtual int addFace(const QList<int>& vertexIndices) = 0;
    virtual int addFaceUniform(const QString& name, QVariant::Type type, bool isList = false) = 0;
    virtual void setFaceUniform(int faceId, int uniformId, const QList<QVariant>& data ) = 0;

    virtual int addUniform(const QString& name, QVariant::Type type, bool isList = false) = 0;
    virtual void setUniform(int uniformId, const QList<QVariant>& data ) = 0;

};

#endif // MESHCONTAINER_H
