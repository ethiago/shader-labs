#ifndef PLANE_H
#define PLANE_H

#include "Object3D.h"

class Plane : public Object3D
{
    int indexList;

public:
    explicit Plane(int staks, int slices);
    ~Plane();

private:
    void drawGeometry(void) const;

    void storeList() const;
};

#endif // PLANE_H
