#include "Point.h"
#include <QVector3D>

#ifdef __APPLE__
    #include <OpenGL/gl.h>
#else
    #include <GL/gl.h>
#endif

Point::Point(const QVector3D& center):
    Object3D(center)
{
    setInputType(GL_POINTS);
}

Point::Point(const Point& p) : Object3D(p)
{
}


Object3D* Point::copy() const
{
    return new Point(*this);
}

Point::~Point()
{
}

void Point::drawGeometry(void)const
{
    glBegin(GL_POINTS);
    glVertex3i(0,0,0);
    glEnd();
}
