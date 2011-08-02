#include <QtOpenGL>
#include "Sphere.h"
#include "Global.h"

Sphere::Sphere(float radius, const QVector3D& center, QObject *parent):
    Object3D(center, parent), m_radius(radius)
{
}

Sphere::Sphere(const Sphere& sph): Object3D(sph), m_radius(sph.radius())
{

}

Sphere::Sphere(int stacks, int slices) : Object3D()
{
    setStacks(stacks);
    setSlices(slices);
}

void Sphere::setRadius(float radius)
{
    m_radius = radius;
}

float Sphere::radius(void)const
{
    return m_radius;
}

void Sphere::drawGeometry(void)const
{
    GLUquadric *quad = gluNewQuadric();

    gluQuadricDrawStyle(quad, GLU_FILL);
    gluQuadricNormals(quad, GLU_SMOOTH);

    gluQuadricTexture(quad, true);
    gluSphere(quad, radius(), slices(), stacks());
}
