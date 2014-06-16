#ifndef DEFAULTNORMAL_H
#define DEFAULTNORMAL_H

#include <QObject>
#include <PerVertexInterface.h>

class DefaultNormal : public QObject, PerVertexInterface
{
    Q_OBJECT
    Q_INTERFACES(PerVertexInterface)

public:
    DefaultNormal();

    QStringList perVertexPlugins() const;

    /******Special Attribute Names**********
     *geometry: must be Vec3 and changes the x,y,z vertex position
     *normal: must be Vec3 and changes the x,y,z normal coordinates
     *texCoord: must be Vec2 and changes the u,v texture coordinates
     *null: does not generate any attributes data/
     * Attribute names can't start with gl_ */
    QList< QPair<QString, DataType> > attributes(const QString& plugin) const;

    QList<QVariant> attributeValue(const QString& plugin, int attIndex, const heds::Vertex* vertex);
    void endOfVertices(const QString& plugin, int attIndex);
};

#endif // DEFAULTNORMAL_H
