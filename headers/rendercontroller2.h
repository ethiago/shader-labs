#ifndef RENDERCONTROLLER2_H
#define RENDERCONTROLLER2_H

#include "Global.h"

#include <QObject>
#include <QPoint>
#include <QGLContext>
#include <QAction>
#include <QMap>
#include <QList>
#include <QPair>


class DirectionalLight;
class MainWindow;
class Scene3D;
class Object3D;
class ArcBall;

class GLDisplay;
class GlobalProperties;

class SLObject;
class SLShaderController;
class SLTextureController;
class Project;
class ObjectController;

class RenderController2 : public QObject
{
    Q_OBJECT

    SLShaderController * e_shaderController;
    SLTextureController * e_textureController;
    ObjectController *m_objectController;
    MainWindow* e_mainwindow;

    QAction * m_loadModel;

    GLDisplay *m_display;
    Scene3D * m_scene;
    ArcBall* m_arcBall;
    DirectionalLight* m_light;
    bool lightRotation;

    int selectedObject;

    QList< QPair<QAction*, Object3D*> > m_models;
    typedef QMap<QAction*, Object3D*> MMap;

    QList<GLenum> primitives;
    GlobalProperties *m_propertries;

    int maxStaticModels;

public:
    explicit RenderController2(MainWindow *mainWindow, SLShaderController * shaderController,
                              SLTextureController* texture);
    ~RenderController2();
    void updateGL(void);
    int getModelId(void);
    void setModelById(int i);
    QVector3D getLightPosition()const;
    Scene3D * getScene();
    void loadModel(const QString& fn);

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
    void origin(bool);
    void objectsVisibility(bool);
    bool newSLObject();
    void projectOpened(Project*);
    void objectChanged(int idx);
    void loadModel();

private:
    void configureModelsAndActions(QMenu*);
    void changeCurrent(Object3D*);

signals:
    void objectChanged();


};

#endif // RENDERCONTROLLER2_H
