#ifndef RENDERCONTROLLER_H
#define RENDERCONTROLLER_H

#include <QObject>
#include <QPoint>

class MainWindow;
class Object3D;
class ArcBall;

namespace ShaderLab
{
    class GLDisplay;
}

using namespace ShaderLab;

class RenderController : public QObject
{
    Q_OBJECT

    GLDisplay *display;
    Object3D * model;
    ArcBall *arcBall;

public:
    explicit RenderController(MainWindow *mainWindow,
                              QObject *parent = 0);
    ~RenderController();

public slots:
    void updateGL(void);
    void drawModel(void);
    void mouseRigthMove(QPoint ini, QPoint curr);
    void mouseRigthFinish(QPoint ini, QPoint curr);
    void mouseLeftMove(QPoint ini, QPoint curr);
    void mouseLefthFinish(QPoint ini, QPoint curr);
    void mouseCancel();
    void textureFileName(const QString&);
    void wireFrameToggle(bool);
    void removeTexture(void);
    void saveResultAsImage();

};

#endif // RENDERCONTROLLER_H
