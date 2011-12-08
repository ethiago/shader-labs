#ifndef RENDERCONTROLLER_H
#define RENDERCONTROLLER_H

#include <QObject>
#include <QPoint>
#include <QGLContext>
#include <QAction>
#include <QMap>
#include <QList>
#include <QPair>

class DirectionalLight;
class MainWindow;
class Object3D;
class ArcBall;

class GLDisplay;
class PrimitivesDialog;
class GlobalProperties;

class TextureController;

class RenderController : public QObject
{
    Q_OBJECT

    GLDisplay *display;
    Object3D * model;
    ArcBall* arcBall;
    DirectionalLight* light;
    bool wireframe;
    bool lightRotation;

    QList< QPair<QAction*, Object3D*> > models;
    typedef QMap<QAction*, Object3D*> MMap;

    QList<GLenum> primitives;
    PrimitivesDialog *primitivesDialog;
    GlobalProperties *propertries;

public:
    explicit RenderController(MainWindow *mainWindow,
                              QObject *parent = 0);
    ~RenderController();
    QGLWidget* getGLContext(void);
    void updateGL(void);
    GLenum getCurrentOutputPrimitive(void);
    GLenum getCurrentInputPrimitive(void);
    int getModelId(void);
    void setModelById(int i);
    QVector3D getLightPosition()const;


public slots:
    void drawModel(void);
    void mouseRigthMove(QPoint ini, QPoint curr);
    void mouseRigthFinish(QPoint ini, QPoint curr);
    void mouseLeftMove(QPoint ini, QPoint curr);
    void mouseLefthFinish(QPoint ini, QPoint curr);
    void mouseCancel();
    void wireFrameToggle(bool);
    void saveResultAsImage();
    void modelChanged(QAction*);
    void showPrimitiveSelector(void);
    void lightSetup(void);
    void lightRotationToggle(bool);    


private:
    void configureModelsAndActions(QMenu*);
    QStringList outPrimitiveSetup(void);


};

#endif // RENDERCONTROLLER_H
