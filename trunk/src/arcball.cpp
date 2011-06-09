#include <cmath>
#include "arcball.h"
#include <QVector2D>
#include <QDebug>
#include <QToolTip>

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

QVector3D ArcBall::mouseToSphere(const QPoint& center, const QPoint& point)
{
    QVector2D v;
    QVector2D v2d;
    float z;
    int quad;

    v2d.setX((point - center).x()/radius());
    v2d.setY((point - center).y()/radius());

    quad = ((int)(v2d.length()))%4;

    QVector2D v2Norm = v2d.normalized();
    v = (v2d - v2Norm*quad);

    switch(quad){
        case 0:
            z = sqrt(1.0 - v.lengthSquared());
            break;
        case 1:
            v = v2Norm - v;
            z = -sqrt(1.0 - v.lengthSquared());;
            break;
        case 2:
            v = -v;
            z = -sqrt(1.0 - v.lengthSquared());;
            break;
        case 3:
            v = -(v2Norm - v);
            z = sqrt(1.0 - v.lengthSquared());
            break;
        default:
            qDebug() << "Quad out of range";
    }

    return QVector3D(v, z);
}
