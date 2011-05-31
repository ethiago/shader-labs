#include <cmath>
#include "arcball.h"

ArcBall::ArcBall(float radius, QObject *parent) :
    QObject(parent), m_radius(radius)
{
}

void ArcBall::setRadius(float radius)
{
    m_radius = radius;
}

float ArcBall::radius(void)const
{
    return m_radius;
}

QQuaternion ArcBall::rotationForPoints(const QPoint& center,
                              const QPoint& begin,
                              const QPoint& end)
{
    QVector3D v0;
    QVector3D v1;

    v0 = mouseToSphere(center, begin);
    v1 = mouseToSphere(center, end);

    QVector3D vc = QVector3D::crossProduct(v0,v1);

    return QQuaternion(QVector3D::dotProduct(v0,v1),vc);
}

QQuaternion ArcBall::rotationForPoints(float radius,
                                     const QPoint& center,
                                     const QPoint& begin,
                                     const QPoint& end)
{
    return QQuaternion();
}

QVector3D ArcBall::mouseToSphere(const QPoint& center, const QPoint& point)
{
    QVector3D v;

    v.setX((point.x() - center.x())/m_radius);
    v.setY((point.y() - center.y())/m_radius);

    float r = v.x()*v.x() + v.y()*v.y();
    if(r > 1.0)
    {
        float s = 1.0/sqrt(r);
        v.setX( v.x()*s );
        v.setY( v.y()*s );
        v.setZ(0.0);
    }else
    {
        v.setZ(sqrt(1.0 - r));
    }

    return v;
}
