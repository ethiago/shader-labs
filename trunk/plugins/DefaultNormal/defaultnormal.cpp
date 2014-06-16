#include "defaultnormal.h"
#include <QStringList>
#include <QVariant>
#include "vertex.h"
#include "face.h"


DefaultNormal::DefaultNormal()
{

}

QStringList DefaultNormal::perVertexPlugins()const
{
    QStringList st;
    st.append(QString("Normal Calculation"));
    return st;
}

QList< QPair<QString, DataType> > DefaultNormal::attributes(const QString& plugin) const
{
    QList< QPair<QString, DataType> > list;
    list.append( qMakePair(QString("normal"), Vec3) );
    return list;
}

QList<QVariant> DefaultNormal::attributeValue(const QString& plugin, int attIndex, const heds::Vertex *vertex)
{
    heds::Vertex::iterator it(vertex->begin());
    QVector3D normal;

    for(; it != vertex->end(); ++it)
    {
        const heds::HalfEdge * he = *it;
        if(he->face() == NULL)
        {
            continue;
        }
        normal += he->face()->getNormal();
    }

    normal.normalize();

    QList<QVariant> list;

    list.append(QVariant(normal.x()));
    list.append(QVariant(normal.y()));
    list.append(QVariant(normal.z()));

    return list;
}

void DefaultNormal::endOfVertices(const QString& plugin, int attIndex)
{

}

Q_EXPORT_PLUGIN2(defaultnormalplugin, DefaultNormal)
