#include "line.h"
#include "Global.h"

#include <QtOpenGL>
#ifdef __APPLE__
    #include <OpenGL/glu.h>
#else
    #include <GL/glu.h>
#endif

Line::Line():
    Object3D()
{
    indexList = glGenLists(1);
    setInputType(GL_LINE_STRIP);
    storeList();
}

Line::Line(const Line &rhs): Object3D(rhs)
{
    indexList = glGenLists(1);
    storeList();
}

Object3D* Line::copy() const
{
    return new Line(*this);
}

Line::~Line()
{
    glDeleteLists(indexList,1);
}

void Line::drawGeometry(void)const
{
    glCallList(indexList);
}

void Line::storeList()
{
    float v[33] = {   -1.0, 0.0,  0.0,
                      -0.8, 0.0, 0.0,
                      -0.6, 0.0, 0.0,
                      -0.4, 0.0, 0.0,
                      -0.2, 0.0, 0.0,
                       0.0, 0.0, 0.0,
                       0.2, 0.0, 0.0,
                       0.4, 0.0, 0.0,
                       0.6, 0.0, 0.0,
                       0.8, 0.0, 0.0,
                       1.0, 0.0, 0.0};

    glColor4f( 0.35, 0.35, 0.35, 1.0);
    glNewList(indexList, GL_COMPILE);
    glBegin(GL_LINE_STRIP);
    {
        for(int i=0; i < 11; ++i)
        {
            glVertex3fv(&v[3*i]);
        }
    }glEnd();
    glEndList();
}
