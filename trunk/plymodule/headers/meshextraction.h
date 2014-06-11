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

    QString m_normalXPropertyName;
    QString m_normalYPropertyName;
    QString m_normalZPropertyName;

    QString m_texUPropertyName;
    QString m_texVPropertyName;

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

    void setNormalXName(const QString& value);
    void setNormalYName(const QString& value);
    void setNormalZName(const QString& value);

    void setTexUName(const QString& value);
    void setTexVName(const QString& value);

    void setFaceElementName(const QString& value);
    void setFaceVerticesIndicesName(const QString& value);

    void setUniformElementName(const QString& value);

    virtual bool extract();

private:
    QList<int> vI;
    QList<int> fI;

    bool extractGeometry();
    bool extractTopology();
    void extractNormals();
    void extractTextureCoordinates();
    void extractAttributes();
    void extractFaceUniforms();
    void extractUniforms();

};

#endif // MESHEXTRACTION_H
