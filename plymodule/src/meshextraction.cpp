#include "meshextraction.h"

MeshExtraction::MeshExtraction(const PLYData &data, MeshContainer *container) :
    m_data(data), e_container(container),
    m_vertexElementName(QString("vertex")),
    m_xPropertyName(QString("x")),
    m_yPropertyName(QString("y")),
    m_zPropertyName(QString("z")),
    m_normalXPropertyName(QString("nx")),
    m_normalYPropertyName(QString("ny")),
    m_normalZPropertyName(QString("nz")),
    m_texUPropertyName(QString("u")),
    m_texVPropertyName(QString("v")),
    m_faceElementName(QString("face")),
    m_facePropertyName(QString("vertex_indices")),
    m_uniformElementName(QString("uniform"))

{
}

MeshExtraction::~MeshExtraction()
{
}

void MeshExtraction::setVertexElementName(const QString& value)
{
    m_vertexElementName = value;
}

void MeshExtraction::setXPropertyName(const QString& value)
{
    m_xPropertyName = value;
}

void MeshExtraction::setYPropertyName(const QString& value)
{
    m_yPropertyName = value;
}

void MeshExtraction::setZPropertyName(const QString& value)
{
    m_zPropertyName = value;
}

void MeshExtraction::setNormalXName(const QString& value)
{
    m_normalXPropertyName = value;
}

void MeshExtraction::setNormalYName(const QString& value)
{
    m_normalYPropertyName = value;
}

void MeshExtraction::setNormalZName(const QString& value)
{
    m_normalZPropertyName = value;
}

void MeshExtraction::setTexUName(const QString& value)
{
    m_texUPropertyName = value;
}

void MeshExtraction::setTexVName(const QString& value)
{
    m_texVPropertyName = value;
}

void MeshExtraction::setFaceElementName(const QString& value)
{
    m_faceElementName = value;
}

void MeshExtraction::setFaceVerticesIndicesName(const QString& value)
{
    m_facePropertyName = value;
}

void MeshExtraction::setUniformElementName(const QString& value)
{
    m_uniformElementName = value;
}

bool MeshExtraction::extractGeometry()
{
    vI.clear();

     int vL = m_data.elementLocation(m_vertexElementName);
    if(vL < 0)
        return false;

    PLYDataHeader::Element v = m_data[vL];

    int xL = v.propertyLocation(m_xPropertyName);
    if(xL < 0)
        return false;

    if(v[xL].isList())
        return false;

    int yL = v.propertyLocation(m_yPropertyName);
    if(yL < 0)
        return false;

    if(v[yL].isList())
        return false;

    int zL = v.propertyLocation(m_zPropertyName);
    if(zL >= 0)
    {
        if(v[zL].isList())
            return false;
    }


    for(unsigned int i = 0; i < v.count(); ++i)
    {
        float xValue = m_data.getData(vL, xL, i).at(0).value<float>();
        float yValue = m_data.getData(vL, yL, i).at(0).value<float>();
        float zValue = 0.0;
        if(zL >= 0)
            zValue = m_data.getData(vL, zL, i).at(0).value<float>();

        vI.append( e_container->addVertex(xValue, yValue, zValue) );
    }

    return true;
}

bool MeshExtraction::extractTopology()
{
    fI.clear();
    int fL = m_data.elementLocation(m_faceElementName);
    if(fL < 0)
        return false;

    PLYDataHeader::Element f = m_data[fL];

    int idxL = f.propertyLocation(m_facePropertyName);
    if(idxL < 0)
        return false;

    if(!f[idxL].isList())
        return false;

    for(unsigned int i = 0; i< f.count(); ++i)
    {
        QList<int> idxValue;

        QList<QVariant> list = m_data.getData(fL, idxL, i);

        for(int j = 0; j < list.size(); ++j)
        {
            int value = list[j].value<int>();
            idxValue.append(value);
        }

        fI.append( e_container->addFace(idxValue) );
    }

    return true;
}

void MeshExtraction::extractNormals()
{
    int vL = m_data.elementLocation(m_vertexElementName);
    PLYDataHeader::Element v = m_data[vL];

    int xL = v.propertyLocation(m_normalXPropertyName);
    if(xL < 0)
        return;
    if(v[xL].isList())
        return;

    int yL = v.propertyLocation(m_normalYPropertyName);
    if(yL < 0)
        return;
    if(v[yL].isList())
        return;

    int zL = v.propertyLocation(m_normalZPropertyName);
    if(zL < 0)
        return;
    if(v[zL].isList())
        return;

    for(unsigned int i = 0; i < v.count(); ++i)
    {
        float xValue = m_data.getData(vL, xL, i).at(0).value<float>();
        float yValue = m_data.getData(vL, yL, i).at(0).value<float>();
        float zValue = m_data.getData(vL, zL, i).at(0).value<float>();

        e_container->setNormal(vI[i], xValue, yValue, zValue);
    }
}

void MeshExtraction::extractTextureCoordinates()
{
    int vertexL = m_data.elementLocation(m_vertexElementName);
    PLYDataHeader::Element v = m_data[vertexL];

    int uL = v.propertyLocation(m_texUPropertyName);
    if(uL < 0)
        return;
    if(v[uL].isList())
        return;

    int vL = v.propertyLocation(m_texVPropertyName);
    if(vL < 0)
        return;
    if(v[vL].isList())
        return;

    for(unsigned int i = 0; i < v.count(); ++i)
    {
        float uValue = m_data.getData(vertexL, uL, i).at(0).value<float>();
        float vValue = m_data.getData(vertexL, vL, i).at(0).value<float>();

        e_container->setTexCoord(vI[i], uValue, vValue);
    }
}

void MeshExtraction::extractAttributes()
{
    int vL = m_data.elementLocation(m_vertexElementName);
    PLYDataHeader::Element v = m_data[vL];

    int xL = v.propertyLocation(m_xPropertyName);
    int yL = v.propertyLocation(m_yPropertyName);
    int zL = v.propertyLocation(m_zPropertyName);

    int xnL = v.propertyLocation(m_normalXPropertyName);
    int ynL = v.propertyLocation(m_normalYPropertyName);
    int znL = v.propertyLocation(m_normalZPropertyName);

    int tuL = v.propertyLocation(m_texUPropertyName);
    int tvL = v.propertyLocation(m_texVPropertyName);


    for(int i = 0; i < v.getNumberOfProperties(); ++i )
    {
        if(i == xL || i == yL || i == zL ||
                i == xnL || i == ynL || i == znL ||
                i == tuL || i == tvL)
            continue;

        int attIdx = e_container->addAttribute(v[i].name(), QVariant::Type(v[i].type()) );

        for(unsigned int j = 0; j < v.count(); ++j)
        {
            e_container->setAttribute(vI[j], attIdx, m_data.getData(vL, i, j));
        }

    }
}

void MeshExtraction::extractFaceUniforms()
{
    int fL = m_data.elementLocation(m_faceElementName);
    PLYDataHeader::Element f = m_data[fL];

    int viL = f.propertyLocation(m_facePropertyName);

    for(int i = 0; i < f.getNumberOfProperties(); ++i)
    {
        if(i == viL)
            continue;

        int unIdx = e_container->addFaceUniform(f[i].name(), QVariant::Type(f[i].type()));

        for(unsigned int j = 0; j < f.count(); ++j)
        {
            e_container->setFaceUniform(fI[j], unIdx, m_data.getData(fL, i, j));
        }
    }
}

void MeshExtraction::extractUniforms()
{
    int uL = m_data.elementLocation(m_uniformElementName);
    if(uL < 0)
        return;

    PLYDataHeader::Element u = m_data[uL];
    if(u.count() > 1)
        return;

    for(int i = 0; i < u.getNumberOfProperties(); ++i)
    {
        int uIdx = e_container->addUniform(u[i].name(), QVariant::Type(u[i].type()));

        e_container->setUniform(uIdx, m_data.getData(uL, i, 0));
    }
}

bool MeshExtraction::extract()
{
    if(!extractGeometry())
        return false;
    if(!extractTopology())
        return false;

    extractNormals();

    extractTextureCoordinates();

    extractAttributes();

    extractFaceUniforms();

    extractUniforms();

    return true;
}
