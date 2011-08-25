#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include <QMap>
#include <QGLShaderProgram>

#include "Global.h"

class MainWindow;
class FileController;
class Object3D;
class ChooseShaderDialog;
class RenderController;
class TextureController;
class Project;

class MainController : public QObject
{
    Q_OBJECT

    MainWindow *mainWindow;
    QMap<ShaderLab::Shader, FileController*> fileControllers;
    QGLShaderProgram program;
    TextureController *textureController;
    RenderController *renderController;

    ChooseShaderDialog *chooseShaderDialog;

    Project *project;

public:
    explicit MainController(MainWindow *mw, QObject *parent = 0);
    ~MainController();

public slots:
    void slot_closeShaderCode(ShaderLab::Shader);
    void fileChanged(ShaderLab::Shader);
    void programCloseRequest(QCloseEvent* event);
    void runAllActiveShaders(void);
    void saveAll(void);
    void saveFile(ShaderLab::Shader shadertype);
    void saveFileAs(ShaderLab::Shader shadertype);
    void changeTabActivationStatus(ShaderLab::Shader);
    void newShaderActionClicked(void);
    void openShaderActionClicked(void);
    void loadProject(void);

private:

    FileController* getFileControllerByShaderType(ShaderLab::Shader shadertype);
    bool closeShaderCode(ShaderLab::Shader);
    bool openShader(ShaderLab::Shader, QString);
    void glSetup(void);


};

#endif // MAINCONTROLLER_H
