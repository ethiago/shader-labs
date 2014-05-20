#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include "Global.h"
#include <QObject>
#include <QMap>
#include <QCloseEvent>


class MainWindow;
class SLFile;
class Object3D;
class ChooseShaderDialog;
class RenderController2;
class SLTabWidget;
class SLShaderController;
class SLTextureController;
class SLProjectController;

class MainController : public QObject
{
    Q_OBJECT

    MainWindow *e_mainWindow;
    RenderController2 *m_renderController;
    SLShaderController *m_shaderController;
    SLTextureController *m_textureController;
    SLProjectController *m_projectController;


    ChooseShaderDialog *m_chooseShaderDialog;

public:
    explicit MainController(MainWindow *mw, QObject *parent = 0);
    ~MainController();

public slots:
    void programCloseRequest(QCloseEvent* event);
    void newObject();

private:
    void glSetup(void);

};

#endif // MAINCONTROLLER_H
