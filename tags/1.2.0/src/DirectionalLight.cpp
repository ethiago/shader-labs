#include "DirectionalLight.h"
#include "Object3D.h"

#ifdef __APPLE__
    #include <OpenGL/gl.h>
#else
    #include <GL/gl.h>
#endif

DirectionalLight::DirectionalLight(QObject *parent)
    : QObject(parent),
      m_rotations(QMatrix4x4()) ,
      m_interactiveQuartenion(QQuaternion()), showLine(false)
{
}

void DirectionalLight::setRotations(const QMatrix4x4& rotations)
{
    m_rotations = rotations;
}

const QMatrix4x4& DirectionalLight::rotations(void)const
{
    return m_rotations;
}

void DirectionalLight::setInteractiveQuartenion(const QQuaternion& quaternion)
{
    m_interactiveQuartenion = quaternion;
}

const QQuaternion& DirectionalLight::interactiveQuartenion(void)const
{
    return m_interactiveQuartenion;
}

void DirectionalLight::addRotation(const QQuaternion& rotacao)
{
    QMatrix4x4 m;
    m.rotate(Object3D::degreeFromCos(rotacao.scalar()), rotacao.x(), rotacao.y(), rotacao.z());
    m_rotations = m*m_rotations;
}

void DirectionalLight::draw(void) const
{

    QVector3D lp = getLightPosition();

    GLfloat lightPosition[4];
    lightPosition[0] = lp.x();
    lightPosition[1] = lp.y();
    lightPosition[2] = lp.z();
    lightPosition[3] = 0;

    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);
    if(showLine)
    {
        glBegin(GL_LINES);
        glColor3f(1.0,1.0,0.0);
        glVertex3f(0.0,0.0,0.0);
        glVertex3fv(lightPosition);
        glEnd();
    }
    glEnable(GL_LIGHTING);
    glEnable(GL_TEXTURE_2D);

    glLightfv(GL_LIGHT0, GL_POSITION, lightPosition);
}

void DirectionalLight::cleanTransformations()
{
    setInteractiveQuartenion(QQuaternion());
    setRotations(QMatrix4x4());
}

QVector3D DirectionalLight::getLightPosition() const
{
    QQuaternion t = interactiveQuartenion();

    QMatrix4x4 m;
    m.rotate(Object3D::degreeFromCos(t.scalar()), t.x(), t.y(), t.z());
    m = m*m_rotations;

    /* Ainda precisa de solução definitiva: a luz precisa ser passada para coordenadas de camera. */
    return m.mapVector(QVector3D(0,-1000000,0));
}

void DirectionalLight::setShowLine(bool value)
{
    showLine = value;
}
