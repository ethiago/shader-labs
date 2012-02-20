#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include <QMap>
#include <QGLShaderProgram>

#include "Global.h"

class MainWindow;
class SLFile;
class Object3D;
class ChooseShaderDialog;
class RenderController;
class Project;
class EditorController;
class SLTabWidget;

class MainController : public QObject
{
    Q_OBJECT

    MainWindow *mainWindow;
    RenderController *renderController;

    ChooseShaderDialog *chooseShaderDialog;
    SLTabWidget *tabWidget;

public:
    explicit MainController(MainWindow *mw, QObject *parent = 0);
    ~MainController();
    void openProject(const QString&);

public slots:
    void programCloseRequest(QCloseEvent* event);
    void newShaderActionClicked(void);
    void openShaderActionClicked(void);
    void loadProject(void);
    void saveShader();
    void saveShaderAs();
    void newObject();

private:
    void glSetup(void);
    void configureShader(ShaderLab::Shader shaderType, const QString& filePath = QString());

};

#endif // MAINCONTROLLER_H
