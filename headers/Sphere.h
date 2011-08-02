#ifndef SPHERE_H
#define SPHERE_H

#include "Object3D.h"

class Sphere : public Object3D
{
    Q_OBJECT

    float m_radius;
public:
    explicit Sphere(int stacks = 50, int slices = 50, float radius = 1.0,
                    const QVector3D& center = QVector3D(),
                    QObject *parent = 0);
    explicit Sphere(const Sphere& sph);

    void setRadius(float radius);
    float radius(void)const;

private:
    virtual void drawGeometry(void) const;

signals:

public slots:

};

#endif // SPHERE_H
