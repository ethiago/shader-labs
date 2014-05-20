#ifndef SLPROJECTCONTROLLER_H
#define SLPROJECTCONTROLLER_H

#include <QObject>

class Scene3D;
class Project;
class SLShaderController;
class SLTextureController;
class RenderController2;

class SLProjectController : public QObject
{
    Q_OBJECT

    Scene3D * e_scene;
    SLShaderController *e_shaderController;
    SLTextureController* e_textureController;
    RenderController2 * e_renderController;


public:
    SLProjectController(Scene3D *scene,
                        SLShaderController * shaderController,
                        SLTextureController* textureController,
                        RenderController2* renderController);
    ~SLProjectController();

    bool isOpened()const;

signals:
    void projectNameChanged(QString);

public slots:
    bool open();
    bool save();
    bool saveAs();
    bool close();
    //
    void currentChanged();

private:
    bool checkNewFile();
};

#endif // SLPROJECTCONTROLLER_H
