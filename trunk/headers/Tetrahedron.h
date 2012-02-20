#ifndef TETRAHEDRON_H
#define TETRAHEDRON_H

#include "Object3D.h"

class Tetrahedron : public Object3D
{
    int indexList;
public:
    explicit Tetrahedron(const QVector3D& center = QVector3D(),
                    QObject *parent = 0);
    explicit Tetrahedron(const Tetrahedron& tt);
    ~Tetrahedron();

    virtual Object3D* copy() const;

private:
    virtual void drawGeometry(void) const;

    void storeList()const;
};

#endif //TETRAHEDRON_H
