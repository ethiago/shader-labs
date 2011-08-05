#ifndef SHADERCONTROLLER_H
#define SHADERCONTROLLER_H

#include <QObject>
#include "Global.h"

class ShaderController : public QObject
{
    Q_OBJECT

    //QList<Shader*> m_shaders;

public:
    explicit ShaderController(QObject *parent = 0);

signals:

public slots:

};

#endif // SHADERCONTROLLER_H
