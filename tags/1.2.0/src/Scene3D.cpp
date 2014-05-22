#include "Scene3D.h"
#include "SLObject.h"
#include "Cube.h"
#include "Sphere.h"

#ifndef __APPLE__
    #include <GL/glext.h>
#endif


Scene3D::Scene3D(const QVector3D& center) :
    Object3D(center), current(NULL), origin(false), m_objectsVisibility(false)
{
}

Scene3D::~Scene3D()
{
    for(int i = 0; i < objects.size(); ++i)
    {
        delete objects[i];
    }
    objects.clear();
}

Object3D* Scene3D::copy() const
{
    return new Scene3D();
}

void Scene3D::drawGeometry(void) const
{
    if(origin)
        drawOrigin();
    for(int i = 0; i < objects.size(); ++i)
        if( m_objectsVisibility || objects[i] == current)
            objects[i]->draw();
}

void Scene3D::addSLObject(SLObject *obj)
{
    objects.append(obj);
    current = obj;
}

void Scene3D::setObjectToCurrent(Object3D* obj)
{
    if(current)
        current->changeObject(obj);
}

SLObject *Scene3D::currentSLObject(void)
{
    return current;
}

void Scene3D::drawOrigin()const
{
    GLboolean isLighting;
    GLfloat color[4];
    glGetBooleanv(GL_LIGHTING,&isLighting);
    glGetFloatv(GL_CURRENT_COLOR,color);
    glDisable(GL_LIGHTING);
    glDisable(GL_TEXTURE_2D);

    //glUseProgram(0);

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

    glEnable(GL_TEXTURE_2D);
    glColor4fv(color);
    if(isLighting)
        glEnable(GL_LIGHTING);
}

void Scene3D::changeOrigin(bool v)
{
    origin = v;
}

void Scene3D::objectsVisibility(bool v)
{
    m_objectsVisibility = v;
}

SLObject* Scene3D::changeCurrent(int idx)
{
    if(idx >=0 && idx < objects.size())
    {
        current = objects[idx];
        return current;
    }else
        return NULL;
}
