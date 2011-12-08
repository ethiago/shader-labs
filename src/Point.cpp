#include <QtOpenGL>
#include <QVector3D>
#include "Point.h"
#include "Global.h"

Point::Point(const QVector3D& center, QObject *parent):
    Object3D(center, parent)
{
    setInputType(GL_POINTS);
}


Point::~Point()
{
}

void Point::drawGeometry(void)const
{
    glBegin(GL_POINTS);
    glVertex3f(center().x(), center().y(), center().z());
    glEnd();
}
