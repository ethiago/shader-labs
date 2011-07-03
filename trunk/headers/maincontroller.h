#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include <QMap>
#include <QGLShaderProgram>

#include "Global.h"

class MainWindow;
class FileController;
class Object3D;

class MainController : public QObject
{
    Q_OBJECT

public:
    explicit MainController(MainWindow *mw, QObject *parent = 0);
    ~MainController();

public slots:
    void slot_closeShaderCode(ShaderLab::Shader);
    void fileChanged(ShaderLab::Shader);
    void newShaderCode(ShaderLab::Shader);
    void openShaderCode(const QString& filename, ShaderLab::Shader);
    void programCloseRequest(QCloseEvent* event);
    void runAllActiveShaders(void);
    void saveAll(void);
    void saveFile(ShaderLab::Shader shadertype);
    void saveFileAs(ShaderLab::Shader shadertype, const QString&, const QString&);
    void changeTabActivationStatus(ShaderLab::Shader);

signals:
    void updateGL(void);

private:
    MainWindow *mainWindow;
    QMap<ShaderLab::Shader, FileController*> fileControllers;
    QGLShaderProgram program;

    FileController* getFileControllerByShaderType(ShaderLab::Shader shadertype);
    void codeAlreadyOpenProcessor(ShaderLab::Shader shadertype);
    bool closeShaderCode(ShaderLab::Shader);

};

#endif // MAINCONTROLLER_H
