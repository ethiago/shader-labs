#ifndef SCENE3D_H
#define SCENE3D_H

#ifdef __APPLE__
    #include <OpenGL/gl.h>
#else
    #include <GL/gl.h>
    #include <GL/glext.h>
#endif

#include "Object3D.h"
#include <QVector>

class SLObject2;

class Scene3D : public Object3D
{
    SLObject2* current;
    QVector<SLObject2*> objects;
    bool origin;
    bool m_objectsVisibility;

public:
    explicit Scene3D(const QVector3D& center = QVector3D(),
                     QObject *parent = 0);
    virtual ~Scene3D();

    void drawGeometry(void) const;

    void addSLObject(SLObject2* obj);

    void setObjectToCurrent(Object3D*);

    SLObject2* currentSLObject(void);

    virtual Object3D* copy() const;

    void changeOrigin(bool v);

    void objectsVisibility(bool v);

private:
    void drawOrigin()const;

};

#endif // SCENE3D_H
