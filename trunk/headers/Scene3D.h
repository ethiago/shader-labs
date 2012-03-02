#ifndef SCENE3D_H
#define SCENE3D_H

#include "Object3D.h"
#include <QVector>

class SLObject;

class Scene3D : public Object3D
{
    SLObject* current;
    QVector<SLObject*> objects;
    bool origin;

public:
    explicit Scene3D(const QVector3D& center = QVector3D(),
                     QObject *parent = 0);
    virtual ~Scene3D();

    void drawGeometry(void) const;

    void addSLObject(SLObject* obj);

    void setObjectToCurrent(Object3D*);

    SLObject* currentSLObject(void);

    virtual Object3D* copy() const;

    void changeOrigin();

private:
    void drawOrigin()const;

};

#endif // SCENE3D_H
