#include "Object3D.h"
#include "slgl3w.h"

//#include "Global.h"

#ifdef __APPLE__
    #include <OpenGL/gl.h>
#else
    #include <GL/gl.h>
#endif

#include <qmath.h>

double Object3D::degreeFromCos(double _cos)
{
    return (acos(_cos)*180.0)/M_PI;
}

Object3D::Object3D(const QVector3D& center)
    : QObject(NULL), m_center(center),
      m_translation(QVector3D()), m_rotations(QMatrix4x4()) ,
      m_interactiveQuartenion(QQuaternion()),
      m_slices(50), m_stacks (50),
      m_inputType(GL_TRIANGLES), m_modelId(-1)
{
}

Object3D::Object3D(const Object3D& obj)
    : QObject(NULL), m_center(obj.center()),
      m_translation(obj.translation()), m_rotations(obj.rotations()),
      m_interactiveQuartenion(obj.interactiveQuartenion()),
      m_slices(obj.slices()), m_stacks(obj.stacks()),
      m_inputType(obj.inputType()), m_modelId(obj.modelId()),
      m_attributeInfo(obj.m_attributeInfo), m_faceInfo(obj.m_faceInfo),
      m_uniformInfo(obj.m_uniformInfo), m_attLocation(obj.m_attLocation),
      m_faceLocation(obj.m_faceLocation), m_uniformLocation(obj.m_uniformLocation)

{
}

Object3D::~Object3D()
{
}

void Object3D::setRotations(const QMatrix4x4& rotations)
{
    m_rotations = rotations;
}

const QMatrix4x4& Object3D::rotations(void)const
{
    return m_rotations;
}

void Object3D::setInteractiveQuartenion(const QQuaternion& quaternion)
{
    m_interactiveQuartenion = quaternion;
}

const QQuaternion& Object3D::interactiveQuartenion(void)const
{
    return m_interactiveQuartenion;
}

void Object3D::setCenter(const QVector3D& center)
{
    m_center = center;
}

const QVector3D& Object3D::center(void)const
{
    return m_center;
}

void Object3D::setSlices(int slices)
{
    m_slices = slices;
}

int Object3D::slices(void)const
{
    return m_slices;
}

void Object3D::setStacks(int stacks)
{
    m_stacks = stacks;
}

int Object3D::stacks(void)const
{
    return m_stacks;
}
void Object3D::setTraslation(const QVector3D& translation)
{
    m_translation = translation;
}

const QVector3D& Object3D::translation(void)const
{
    return m_translation;
}

void Object3D::setModelId(int id)
{
    m_modelId = id;
}

int Object3D::modelId(void) const
{
    return m_modelId;
}

void Object3D::addRotation(const QQuaternion& rotation)
{
    QMatrix4x4 m;
    m.rotate(Object3D::degreeFromCos(rotation.scalar()), rotation.x(), rotation.y(), rotation.z());
    m_rotations = m*m_rotations;
}

void Object3D::draw(const QColor &cor) const
{
    glMatrixMode(GL_MODELVIEW);
    glPushMatrix();

    glTranslatef(translation().x(), translation().y(), translation().z());
    glTranslatef(center().x(), center().y(), center().z());

    QQuaternion t = interactiveQuartenion();
    glRotatef(Object3D::degreeFromCos(t.scalar()),
              t.x(), t.y(), t.z());



    glMultMatrixd(rotations().constData());

    glColor4f(cor.redF(), cor.greenF(), cor.blueF(), cor.alphaF());

    drawGeometry();

    glPopMatrix();
}

void Object3D::cleanTransformations()
{
    setTraslation(QVector3D());
    setCenter(QVector3D());

    setInteractiveQuartenion(QQuaternion());
    setRotations(QMatrix4x4());
}

int Object3D::inputType(void)const
{
    return m_inputType;
}

void Object3D::setInputType(int input)
{
    m_inputType = input;
}

int Object3D::addAttribute(const PLYDataHeader::Property& p)
{
    m_attributeInfo.append(p);
    m_attLocation.append(0);
    return m_attributeInfo.size()-1;
}

int Object3D::addFaceUniform(const PLYDataHeader::Property& p)
{
    m_faceInfo.append(p);
    m_faceLocation.append(0);
    return m_faceInfo.size()-1;
}

int Object3D::addUniform(const PLYDataHeader::Property& p)
{
    m_uniformInfo.append(p);
    m_uniformLocation.append(0);
    return m_uniformInfo.size()-1;
}

const QList<PLYDataHeader::Property>& Object3D::getAttributeInfos()const
{
    return m_attributeInfo;
}

const QList<PLYDataHeader::Property>& Object3D::getFaceUniformInfos()const
{
    return m_faceInfo;
}
const QList<PLYDataHeader::Property>& Object3D::getUniformInfos()const
{
    return m_uniformInfo;
}

void Object3D::setAttributeLocation(int idx, int location)
{
    Q_ASSERT(idx >=0 && idx < m_attLocation.size());

    m_attLocation[idx] = location;
}

void Object3D::setFaceUniformLocation(int idx, int location)
{
    Q_ASSERT(idx >=0 && idx < m_faceLocation.size());

    m_faceLocation[idx] = location;
}

void Object3D::setUniformLocation(int idx, int location)
{
    Q_ASSERT(idx >=0 && idx < m_uniformLocation.size());

    m_uniformLocation[idx] = location;
}

void Object3D::afterLink(unsigned int programId)
{

}

void Object3D::bindAttribute(int i, const QList<QVariant>& list )
{
    Q_ASSERT(i >= 0 && i < m_attributeInfo.size());

    int attrLoc = m_attLocation[i];
    PLYDataHeader::Property::Type type = m_attributeInfo[i].type();

    if(attrLoc <= 0)
        return;

    if(type != PLYDataHeader::Property::INT && type != PLYDataHeader::Property::FLOAT)
        return;

    if(list.size() == 0 || list.size() > 4)
        return;

    QVector4D values;

    if(type == PLYDataHeader::Property::INT)
    {
        switch(list.size())
        {
        case 1:
            SLGl3W::vertexAttrib(attrLoc, list[0].value<int>(), true);
            break;
        case 2:
            values = QVector4D(list[0].value<int>(),list[1].value<int>(), 0.0, 0.0);
            SLGl3W::vertexAttrib(attrLoc, values.toVector2D(), true);
            break;
        case 3:
            values = QVector4D(list[0].value<int>(),list[1].value<int>(), list[2].value<int>(), 0.0 );
            SLGl3W::vertexAttrib(attrLoc, values.toVector3D(), true);
            break;
        case 4:
            values = QVector4D(list[0].value<int>(),list[1].value<int>(), list[2].value<int>(), list[3].value<int>() );
            SLGl3W::vertexAttrib(attrLoc, values, true);
            break;
        }
    }else
    {
        switch(list.size())
        {
        case 1:
            SLGl3W::vertexAttrib(attrLoc, list[0].value<float>());
            break;
        case 2:
            values = QVector4D(list[0].value<float>(),list[1].value<float>(), 0.0, 0.0);
            SLGl3W::vertexAttrib(attrLoc, values.toVector2D());
            break;
        case 3:
            values = QVector4D(list[0].value<float>(),list[1].value<float>(), list[2].value<float>(), 0.0 );
            SLGl3W::vertexAttrib(attrLoc, values.toVector3D());
            break;
        case 4:
            values = QVector4D(list[0].value<float>(),list[1].value<float>(), list[2].value<float>(), list[3].value<float>() );
            SLGl3W::vertexAttrib(attrLoc, values);
            break;
        }
    }
}

void Object3D::bindUniform(int uniformLoc, const QList<QVariant>&list, PLYDataHeader::Property::Type type )
{
    if(uniformLoc < 0)
        return;

    if(type != PLYDataHeader::Property::INT && type != PLYDataHeader::Property::FLOAT)
        return;

    if(list.size() == 0 || list.size() > 4)
        return;

    QVector4D values;

    if(type == PLYDataHeader::Property::INT)
    {
        switch(list.size())
        {
        case 1:
            SLGl3W::uniform(uniformLoc, list[0].value<int>(), true);
            break;
        case 2:
            values = QVector4D(list[0].value<int>(),list[1].value<int>(), 0.0, 0.0);
            SLGl3W::uniform(uniformLoc, values.toVector2D(), true);
            break;
        case 3:
            values = QVector4D(list[0].value<int>(),list[1].value<int>(), list[2].value<int>(), 0.0 );
            SLGl3W::uniform(uniformLoc, values.toVector3D(), true);
            break;
        case 4:
            values = QVector4D(list[0].value<int>(),list[1].value<int>(), list[2].value<int>(), list[3].value<int>() );
            SLGl3W::uniform(uniformLoc, values, true);
            break;
        }
    }else
    {
        switch(list.size())
        {
        case 1:
            SLGl3W::uniform(uniformLoc, list[0].value<float>());
            break;
        case 2:
            values = QVector4D(list[0].value<float>(),list[1].value<float>(), 0.0, 0.0);
            SLGl3W::uniform(uniformLoc, values.toVector2D());
            break;
        case 3:
            values = QVector4D(list[0].value<float>(),list[1].value<float>(), list[2].value<float>(), 0.0 );
            SLGl3W::uniform(uniformLoc, values.toVector3D());
            break;
        case 4:
            values = QVector4D(list[0].value<float>(),list[1].value<float>(), list[2].value<float>(), list[3].value<float>() );
            SLGl3W::uniform(uniformLoc, values);
            break;
        }
    }
}
