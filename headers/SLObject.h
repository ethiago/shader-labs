#ifndef SLOBJECT_H
#define SLOBJECT_H

#include <QObject>
#include "SLTextures.h"
#include "SLShader.h"
#include "Object3D.h"



class SLObject : public QObject
{
    Q_OBJECT

    SLShader m_shader;
    Object3D *m_object;
    SLTextures textures;

public:
    explicit SLObject(Object3D *obj, QObject *parent = 0);

    void setObject(Object3D *obj);

signals:

public slots:

};

#endif // SLOBJECT_H
