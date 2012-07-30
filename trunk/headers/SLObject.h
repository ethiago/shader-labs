#ifndef SLOBJECT_H
#define SLOBJECT_H

#include "SLTextures.h"
#include "SLShader.h"
#include "Object3D.h"
#include "qglshaderprogram.h"
#include <QObject>

class Project;
class MainWindow;

class SLObject : public QObject
{
    Q_OBJECT

    SLShader *m_shader;
    Object3D *m_object;
    SLTextures textures;

    Project * m_project;

    QGLShaderProgram program;

public:
    explicit SLObject(MainWindow *mw, QObject *parent = 0);
    virtual ~SLObject();

    void setObject(Object3D *obj);
    SLShader* shader();
    void draw();
    void setTexturesFromProject(const QStringList&);

    const QString& shaderLog();

    void setProject(Project *);
    QString saveMerge(bool as);
    void deleteObject3D();


    void close(MainWindow* mw);

    void closeProject();

signals:
    void sendLog(const QString&);

public slots:
    void compileShaders();


};

#endif // SLOBJECT_H
