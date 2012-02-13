#ifndef SCENE3D_H
#define SCENE3D_H

#include "Object3D.h"
#include <QVector>

class Scene3D : public Object3D
{
    Q_OBJECT

    QVector<Object3D*> objects;

public:
    explicit Scene3D(const QVector3D& center = QVector3D(),
                     QObject *parent = 0);

    void drawGeometry(void) const;

    void addObject(Object3D*);
    void removeObject(Object3D*);
    void clearObjects(void);

private:
    void drawOrigin()const;

};

#endif // SCENE3D_H
