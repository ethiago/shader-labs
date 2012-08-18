#ifndef SPHERE_H
#define SPHERE_H

#include <GL/gl.h>
#include <GL/glu.h>
#include "Object3D.h"

class Sphere : public Object3D
{

    float m_radius;
    int indexList;
public:
    explicit Sphere(int stacks = 50, int slices = 50, float radius = 1.0,
                    const QVector3D& center = QVector3D(),
                    QObject *parent = 0);
    explicit Sphere(const Sphere& sph);
    ~Sphere();

    float radius(void)const;

    virtual Object3D* copy() const;

private:
    virtual void drawGeometry(void) const;

    void storeList();

};

#endif // SPHERE_H
