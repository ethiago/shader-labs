#ifndef POINT_H
#define POINT_H

#include "Object3D.h"

class Point : public Object3D
{
    Q_OBJECT

public:
    explicit Point(const QVector3D& center = QVector3D(),
                    QObject *parent = 0);
     ~Point();

private:
    virtual void drawGeometry(void) const;
};

#endif // POINT_H
