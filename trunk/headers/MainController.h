#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include <QMap>
#include <QGLShaderProgram>

#include "Global.h"

class MainWindow;
class FileController;
class Object3D;
class TextureController;
class ChooseShaderDialog;

class MainController : public QObject
{
    Q_OBJECT

public:
    explicit MainController(MainWindow *mw, QObject *parent = 0);
    ~MainController();
    void setTextureController(TextureController *textureController);

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
    void selectedShaderOpenDialog(ShaderLab::Shader);
    void selectedShaderNewDialog(ShaderLab::Shader);

signals:
    void updateGL(void);

private:
    MainWindow *mainWindow;
    QMap<ShaderLab::Shader, FileController*> fileControllers;
    QGLShaderProgram program;
    TextureController *textureController;

    ChooseShaderDialog *choiceDialogNew;  /* The choice dialog for new files. */
    ChooseShaderDialog *choiceDialogOpen; /* The choice dialog for existing files. */

    FileController* getFileControllerByShaderType(ShaderLab::Shader shadertype);
    void codeAlreadyOpenProcessor(ShaderLab::Shader shadertype);
    bool closeShaderCode(ShaderLab::Shader);
    void openShaderCode(ShaderLab::Shader);
    void newShaderCode(ShaderLab::Shader);

};

#endif // MAINCONTROLLER_H
