#ifndef PLANE_H
#define PLANE_H

#include "object3d.h"

class Plane : public Object3D
{
    int indexList;

public:
    explicit Plane();

private:
    void drawGeometry(void) const;

    void storeList();
};

#endif // PLANE_H
