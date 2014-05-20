#ifndef POINT_H
#define POINT_H

#include "Object3D.h"

class Point : public Object3D
{

public:
    explicit Point(const QVector3D& center = QVector3D());
    explicit Point(const Point& p);
     ~Point();

    virtual Object3D* copy() const;

private:
    virtual void drawGeometry(void) const;
};

#endif // POINT_H
