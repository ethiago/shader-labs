#ifndef RENDERCONTROLLER_H
#define RENDERCONTROLLER_H


#include <QObject>
#include <QPoint>
#include <QGLContext>
#include <QAction>
#include <QMap>
#include <QList>
#include <QPair>
#include "Global.h"

class DirectionalLight;
class MainWindow;
class Scene3D;
class Object3D;
class ArcBall;

class GLDisplay;
class PrimitivesDialog;
class GlobalProperties;

class SLObject;
class EditorController;
class Project;
class SLObjectController;

class RenderController : public QObject
{
    Q_OBJECT

    GLDisplay *display;
    Scene3D * scene;
    Object3D * model;
    ArcBall* arcBall;
    DirectionalLight* light;
    bool lightRotation;
    MainWindow *mainWindow;
    SLObjectController * objectController;


    QList< QPair<QAction*, Object3D*> > models;
    typedef QMap<QAction*, Object3D*> MMap;

    QList<GLenum> primitives;
    GlobalProperties *propertries;

public:
    explicit RenderController(MainWindow *mainWindow, SLObjectController * obj,
                              QObject *parent = 0);
    ~RenderController();
    void updateGL(void);
    int getModelId(void);
    void setModelById(int i);
    QVector3D getLightPosition()const;
    void newSLObject();
    EditorController* setShader( ShaderLab::Shader, const QString& filePath = QString());
    void setTexturesFromProject(const QStringList& list);
    bool closeAllFiles();
    void setProject(Project*);
    void closeObject();

public slots:
    void drawModel(void);
    void mouseRigthMove(QPoint ini, QPoint curr);
    void mouseRigthFinish(QPoint ini, QPoint curr);
    void mouseLeftMove(QPoint ini, QPoint curr);
    void mouseLefthFinish(QPoint ini, QPoint curr);
    void mouseCancel();
    void wireFrameToggle();
    void saveResultAsImage();
    void modelChanged(QAction*);
    void lightSetup(void);
    void lightRotationToggle(bool);
    void saveProjectAs(void);
    void saveProject(void);
    void origin(bool);
    void objectsVisibility(bool);


private:
    void configureModelsAndActions(QMenu*);


};

#endif // RENDERCONTROLLER_H
