#ifndef CUBE_H
#define CUBE_H

#include "Object3D.h"

class Cube : public Object3D
{
    int indexList;
public:
    explicit Cube(const QVector3D& center = QVector3D(),
                    QObject *parent = 0);
    explicit Cube(const Cube& sph);
    ~Cube();

    virtual Object3D* copy(void) const;

private:
    virtual void drawGeometry(void) const;

    void storeList()const;

};

#endif // CUBE_H
