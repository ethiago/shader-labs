#ifndef ARCBALL_H
#define ARCBALL_H

#include <QObject>
#include <QQuaternion>
#include <QPoint>
#include <QVector3D>

class ArcBall : public QObject
{
    Q_OBJECT

    float m_radius;
    QPoint m_center;
public:
    explicit ArcBall(float radius = 500.0, QObject *parent = 0);

    void setRadius(float radius);
    float radius(void)const;

    QQuaternion rotationForPoints(const QPoint& center,
                                  const QPoint& begin,
                                  const QPoint& end);


private:
    QVector3D mouseToSphere(const QPoint& center, const QPoint& point);

};

#endif // ARCBALL_H
