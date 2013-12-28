#ifndef SLOBJECT2_H
#define SLOBJECT2_H

#include <QObject>

class Project;
class MainWindow;
class SLShaderProgram;
class Object3D;
class SLTextures;

class SLObject2 : public QObject
{
    Q_OBJECT

    MainWindow * mainwindow;
    SLShaderProgram *m_shader;
    Object3D *m_object;
    SLTextures *m_textures;

    Project * m_project;

public:
    explicit SLObject2(MainWindow *mw, SLShaderProgram* slp, SLTextures* slt, QObject *parent = 0);
    virtual ~SLObject2();
    void setObject(Object3D *obj);
    void draw();
    void setTexturesFromProject(const QStringList&);
    QString shaderLog();
    void setProject(Project *);
    QString saveMerge(bool as);
    void close();
    void closeProject();
    SLTextures * textures();
    void toggleWireframe();
    Object3D * object3D();
    int programId();

signals:
    void sendLog(const QString&);

public slots:
    void compileShaders();


};

#endif // SLOBJECT2_H
