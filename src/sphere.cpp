#include <QtOpenGL>
#include "sphere.h"
#include "global.h"

Sphere::Sphere(float radius, const QVector3D& center, QObject *parent):
    Object3D(center, parent), m_radius(radius)
{
}

Sphere::Sphere(const Sphere& sph): Object3D(sph), m_radius(sph.radius())
{

}

void Sphere::setRadius(float radius)
{
    m_radius = radius;
}

float Sphere::radius(void)const
{
    return m_radius;
}

void Sphere::draw(void)const
{
    GLUquadric *quad = gluNewQuadric();

    QQuaternion t = interactiveQuartenion();

    glPushMatrix();

    glTranslatef(translation().x(), translation().y(), translation().z());
    glTranslatef(center().x(), center().y(), center().z());

    glRotatef(ShaderLab::degreeFromCos(t.scalar()),
              t.x(), t.y(), t.z());

    glMultMatrixd(rotacoes().constData());

    gluQuadricDrawStyle(quad, GLU_FILL);
    gluQuadricNormals(quad, GLU_SMOOTH);
    if(texture() >= 0)
        gluQuadricTexture(quad, texture());
    gluSphere(quad, radius(), slices(), stacks());

    glPopMatrix();
}
