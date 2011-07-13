#ifndef RENDERCONTROLLER_H
#define RENDERCONTROLLER_H

#include <QObject>
#include <QPoint>
#include <QGLContext>
#include <QAction>

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
    QAction* actionSphere;
    QAction* actionPlane;
    QAction* actionSphereST;
    QAction* actionPlaneST;


public:
    explicit RenderController(MainWindow *mainWindow,
                              QObject *parent = 0);
    ~RenderController();
    QGLWidget* getGLContext(void);

public slots:
    void updateGL(void);
    void drawModel(void);
    void mouseRigthMove(QPoint ini, QPoint curr);
    void mouseRigthFinish(QPoint ini, QPoint curr);
    void mouseLeftMove(QPoint ini, QPoint curr);
    void mouseLefthFinish(QPoint ini, QPoint curr);
    void mouseCancel();
    void wireFrameToggle(bool);
    void saveResultAsImage();
    void updateTexture(int);
    void modelChanged(QAction*);


};

#endif // RENDERCONTROLLER_H
