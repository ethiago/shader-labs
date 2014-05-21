#ifndef SCENE3D_H
#define SCENE3D_H

#ifdef __APPLE__
    #include <OpenGL/gl.h>
#else
    #include <GL/gl.h>
    #ifdef _WIN32
        #include <GL/glext.h>
    #endif
#endif

#include "Object3D.h"
#include <QObject>
#include <QVector>

class SLObject;

class Scene3D : public Object3D
{
    SLObject* current;
    QVector<SLObject*> objects;
    bool origin;
    bool m_objectsVisibility;

public:
    explicit Scene3D(const QVector3D& center = QVector3D());
    virtual ~Scene3D();

    void drawGeometry(void) const;

    void addSLObject(SLObject* obj);

    void setObjectToCurrent(Object3D*);

    SLObject* currentSLObject(void);

    virtual Object3D* copy() const;

    void changeOrigin(bool v);

    void objectsVisibility(bool v);

    SLObject* changeCurrent(int idx);

private:
    void drawOrigin()const;

};

#endif // SCENE3D_H
