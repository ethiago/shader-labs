#ifndef RENDERCONTROLLER_H
#define RENDERCONTROLLER_H

#include <QObject>
#include <QPoint>
#include <QGLContext>
#include <QAction>
#include <QMap>

#include "PrimitivesDialog.h"

class MainWindow;
class Object3D;
class ArcBall;

namespace ShaderLab
{
    class GLDisplay;
}

using namespace ShaderLab;

class TextureController;

class RenderController : public QObject
{
    Q_OBJECT

    GLDisplay *display;
    Object3D * model;
    ArcBall* arcBall;
    bool wireframe;

    QMap<QAction*, Object3D*> models;
    typedef QMap<QAction*, Object3D*> MMap;

    QList<int> primitives;
    PrimitivesDialog *primitivesDialog;

public:
    explicit RenderController(MainWindow *mainWindow,
                              QObject *parent = 0);
    ~RenderController();
    QGLWidget* getGLContext(void);
    void updateGL(void);
    int getCurrentOutputPrimitive(void);
    int getCurrentInputPrimitive(void);



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


private:
    void configureModelsAndActions(QMenu*);
    QStringList primitiveSetup(void);

};

#endif // RENDERCONTROLLER_H
