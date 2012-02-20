#include <QtOpenGL>
#include <QVector3D>
#include "Point.h"
#include "Global.h"

Point::Point(const QVector3D& center, QObject *parent):
    Object3D(center, parent)
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
