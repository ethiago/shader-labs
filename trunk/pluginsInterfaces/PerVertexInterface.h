#ifndef PERVERTEXINTERFACE_H
#define PERVERTEXINTERFACE_H

#include <QtPlugin>
#include <QPair>
#include "vertex.h"

class QStringList;
class QString;
class QVariant;

enum DataType{
    Int,
    Float,
    Vec2,
    IVec2,
    Vec3,
    IVec3,
    Vec4,
    IVec4
};

class PerVertexInterface
{

public:
    virtual ~PerVertexInterface() {}

    //virtual void setData(const heds::HalfEdgeData& data) = 0;

    virtual QStringList perVertexPlugins() const = 0;

    /******Special Attribute Names**********
     *geometry: must be Vec3 and changes the x,y,z vertex position
     *normal: must be Vec3 and changes the x,y,z normal coordinates
     *texCoord: must be Vec2 and changes the u,v texture coordinates
     *null: does not affect
     * Attribute names can't start with gl_ */
    virtual QList< QPair<QString, DataType> > attributes(const QString& plugin) const = 0;

    virtual QList<QVariant> attributeValue(const QString& plugin, int attIndex, const heds::Vertex* vertex) = 0;
    virtual void endOfVertices(const QString& plugin, int attIndex) = 0;
};

Q_DECLARE_INTERFACE(PerVertexInterface,
                    "br.ufrj.ShaderLabs.PerVertexInterface/1.0")

#endif // PERVERTEXINTERFACE_H
