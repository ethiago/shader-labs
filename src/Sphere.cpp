#include "Sphere.h"
#include "Global.h"

#include <QtOpenGL>
#include <GL/glu.h>

Sphere::Sphere(int stacks, int slices, float radius, const QVector3D& center, QObject *parent):
    Object3D(center, parent), m_radius(radius)
{
    setStacks(stacks);
    setSlices(slices);

    indexList = glGenLists(1);
    setInputType(GL_TRIANGLES);
    storeList();
}

Sphere::Sphere(const Sphere& sph): Object3D(sph), m_radius(sph.radius())
{
    indexList = glGenLists(1);
    storeList();
}

Object3D* Sphere::copy() const
{
    return new Sphere(*this);
}

Sphere::~Sphere()
{
    glDeleteLists(indexList,1);
}

void Sphere::drawGeometry(void)const
{
    glCallList(indexList);
}

void Sphere::storeList()
{
    GLUquadric *quad = gluNewQuadric();

    gluQuadricDrawStyle(quad, GLU_FILL);
    gluQuadricNormals(quad, GLU_SMOOTH);

    gluQuadricTexture(quad, true);

    glNewList(indexList, GL_COMPILE);
        gluSphere(quad, radius(), slices(), stacks());
    glEndList();
}

float Sphere::radius(void)const
{
    return m_radius;
}
