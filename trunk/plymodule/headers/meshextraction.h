#ifndef MESHEXTRACTION_H
#define MESHEXTRACTION_H

#include "plydata.h"
#include "meshcontainer.h"

class MeshExtraction
{

protected:
    PLYData m_data;
    MeshContainer * e_container;

    QString m_vertexElementName;
    QString m_xPropertyName;
    QString m_yPropertyName;
    QString m_zPropertyName;

    QString m_faceElementName;
    QString m_facePropertyName;

    QString m_uniformElementName;

public:
    MeshExtraction(const PLYData& data, MeshContainer* container);
    ~MeshExtraction();

    void setVertexElementName(const QString& value);
    void setXPropertyName(const QString& value);
    void setYPropertyName(const QString& value);
    void setZPropertyName(const QString& value);

    void setFaceElementName(const QString& value);
    void setFaceVerticesIndicesName(const QString& value);

    void setUniformElementName(const QString& value);

    virtual bool extract();

private:
    QList<int> vI;
    QList<int> fI;

    bool extractGeometry();
    bool extractTopology();
    void extractAttributes();
    void extractFaceUniforms();
    void extractUniforms();

};

#endif // MESHEXTRACTION_H
