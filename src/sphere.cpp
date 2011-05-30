#include <QtOpenGL>
#include "sphere.h"

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

    glPushMatrix();
    glTranslatef(translation().x(), translation().y(), translation().z());
    glTranslatef(center().x(), center().y(), center().z());
    gluSphere(quad, radius(), slices(), stacks());
    glPopMatrix();
}
