#include <QtOpenGL>
#include "Scene3D.h"
#include "Cube.h"
#include "Sphere.h"

Scene3D::Scene3D(const QVector3D& center, QObject *parent) :
    Object3D(center, parent)
{
}

void Scene3D::drawGeometry(void) const
{
    drawOrigin();
    for(int i = 0; i < objects.size(); ++i)
        objects.at(i)->draw();
}

void Scene3D::addObject(Object3D* obj)
{
    objects.append(obj);
}

void Scene3D::clearObjects(void)
{
    objects.clear();
}

void Scene3D::removeObject(Object3D* obj)
{
    for(int i = 0; i < objects.size(); ++i)
        if(objects[i] == obj)
            objects.remove(i);
}

void Scene3D::drawOrigin()const
{
    GLboolean isLighting;
    GLfloat color[4];
    glGetBooleanv(GL_LIGHTING,&isLighting);
    glGetFloatv(GL_CURRENT_COLOR,color);
    glDisable(GL_LIGHTING);

    glBegin(GL_LINES);
    glColor3f(1.0, 0, 0);
    glVertex3i(0, 0, 0);
    glVertex3i(1, 0, 0);

    glColor3f(0, 1.0, 0);
    glVertex3i(0, 0, 0);
    glVertex3i(0, 1, 0);

    glColor3f(0, 0, 1.0);
    glVertex3i(0, 0, 0);
    glVertex3i(0, 0, 1);
    glEnd();

    glColor4fv(color);
    if(isLighting)
        glEnable(GL_LIGHTING);
}
