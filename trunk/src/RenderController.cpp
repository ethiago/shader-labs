#include "RenderController.h"
#include "GLDisplay.h"
#include "SLObject2.h"
#include "MainWindow.h"
#include "Object3D.h"
#include "Sphere.h"
#include "Plane.h"
#include "Cube.h"
#include "Point.h"
#include "Scene3D.h"
#include "Tetrahedron.h"
#include "PointPatch.h"
#include "IcosahedronPatch.h"
#include "Arcball.h"
#include "DirectionalLight.h"
#include "PrimitivesDialog.h"
#include "GlobalProperties.h"
#include "InterfaceRequests.h"
#include "Project.h"
#include "SLObjectController.h"
#include "SLShaderProgramController.h"

#include <QToolTip>
#include <QDebug>
#include <QtOpenGL>
#include <QPoint>

RenderController::RenderController(MainWindow *mainWindow,SLObjectController * obj,
                                   QObject *parent):
    QObject(parent)
{
    this->mainWindow = mainWindow;
    objectController = obj;
    scene = new Scene3D();
    light = new DirectionalLight;
    arcBall = new ArcBall(500);
    lightRotation = false;
    selectedObject = -1;

    this->display = new GLDisplay(new QGLContext(QGLFormat::defaultFormat()));
    mainWindow->setGLDisplay(display);

    {  // esta ordem deve ser mantida
        display->updateGL();

        configureModelsAndActions(mainWindow->modelsMenu());

        connect(display, SIGNAL(drawModel()),
                this, SLOT(drawModel()));

        connect(display, SIGNAL(lightSetup()),
                this, SLOT(lightSetup()));
    }


    { // configuracao das propriedades globais
        propertries = new GlobalProperties(mainWindow);
        propertries->setWidget(display->getPropertyBrowser());
        mainWindow->addDockWidget(Qt::RightDockWidgetArea, propertries);
        propertries->close();
    }

    connect(display, SIGNAL(mouseLefthFinish(QPoint,QPoint)),
            this, SLOT(mouseLefthFinish(QPoint,QPoint)));

    connect(display, SIGNAL(mouseLeftMove(QPoint,QPoint)),
            this, SLOT(mouseLeftMove(QPoint,QPoint)));

    connect(display, SIGNAL(mouseRigthFinish(QPoint,QPoint)),
            this, SLOT(mouseRigthFinish(QPoint,QPoint)));

    connect(display, SIGNAL(mouseRigthMove(QPoint,QPoint)),
            this, SLOT(mouseRigthMove(QPoint,QPoint)));

    connect(display, SIGNAL(mouseCancel()),
            this, SLOT(mouseCancel()));

    connect(mainWindow, SIGNAL(wireframeClicked()),
            this, SLOT(wireFrameToggle()));

    connect(mainWindow, SIGNAL(saveResultAsImage()),
            this, SLOT(saveResultAsImage()));

    connect(mainWindow, SIGNAL(lightRotationToggle(bool)),
            this, SLOT(lightRotationToggle(bool)));

    connect(mainWindow, SIGNAL(saveAsProject()),
            this, SLOT(saveProjectAs()));

    connect(mainWindow, SIGNAL(saveProject()),
            this, SLOT(saveProject()));

    connect(mainWindow, SIGNAL(origin(bool)),
            this, SLOT(origin(bool)));

    connect(mainWindow, SIGNAL(objectsVisibility(bool)),
            this, SLOT(objectsVisibility(bool)));

    newSLObject();
}

RenderController::~RenderController()
{
    delete scene;
    for(int i = 0 ; i < models.size(); ++i)
    {
        Object3D* obj = models[i].second;
        delete obj;
    }
    models.clear();

    delete display;
    delete arcBall;
}

void RenderController::updateGL(void)
{
    display->updateGL();
}

void RenderController::lightSetup(void)
{
    light->draw();
}

void RenderController::drawModel(void)
{
    scene->draw();
}

void RenderController::mouseRigthMove(QPoint begin, QPoint curr)
{
    QVector3D v(curr-begin);
    scene->setTraslation(v/40.0);
    display->updateGL();
}

void RenderController::mouseRigthFinish(QPoint ini, QPoint curr)
{
    QVector3D v(curr-ini);
    scene->setTraslation(QVector3D());
    scene->setCenter(scene->center() + (v/40.0));
    display->updateGL();
}

void RenderController::mouseLeftMove(QPoint ini, QPoint curr)
{
    if(ini == curr)
        return;
    QPoint center(display->width()/2, display->height()/2);

    if(lightRotation)
        light->setInteractiveQuartenion(
                arcBall->rotationForPoints(center,curr, ini));
    else
        scene->setInteractiveQuartenion(
                arcBall->rotationForPoints(center,curr, ini));

    display->updateGL();
}

void RenderController::mouseLefthFinish(QPoint ini, QPoint curr)
{
    if(ini == curr)
        return;
    QPoint center(display->width()/2, display->height()/2);
    QQuaternion t = arcBall->rotationForPoints(center,curr, ini);
    if(lightRotation)
    {
        light->addRotation(t);
        light->setInteractiveQuartenion(QQuaternion());
    }else
    {
        scene->addRotation(t);
        scene->setInteractiveQuartenion(QQuaternion());
    }

    display->updateGL();
}

void RenderController::mouseCancel()
{
    if(lightRotation)
        light->setInteractiveQuartenion(QQuaternion());
    else
        scene->setInteractiveQuartenion(QQuaternion());

    display->updateGL();
}

void RenderController::wireFrameToggle()
{
    SLObject2 * current = scene->currentSLObject();
    current->toggleWireframe();
    display->updateGL();
}

void RenderController::saveResultAsImage()
{
    QString filePath = InterfaceRequests::saveImage();

    if(filePath.isEmpty())
        return;

    display->updateGL();
    display->grabFrameBuffer().save(filePath);
}

void RenderController::modelChanged(QAction* action)
{
    for(int i = 0; i < models.size(); ++i)
    {
        models[i].first->setChecked(false);
        if(models[i].first == action)
        {
           models[i].first->setChecked(true);
           scene->setObjectToCurrent(models[i].second->copy());
           selectedObject = i;
        }
    }

    display->updateGL();
}

void RenderController::configureModelsAndActions(QMenu* menu)
{
    QAction* act;
    Object3D *model_tmp;

    act = menu->addAction(tr("&Sphere"));
    act->setCheckable(true);
    model_tmp = new Sphere(50,50);
    act->setChecked(true);          // comeca pela esfera
    model_tmp->setModelId(models.size());
    selectedObject = 0;
    scene->setObjectToCurrent(model_tmp->copy());
    models.append(qMakePair(act, model_tmp));

    act = menu->addAction(tr("&Plane"));
    act->setCheckable(true);
    act->setChecked(false);
    model_tmp = new Plane(50,50);
    model_tmp->setModelId(models.size());
    models.append(qMakePair(act, model_tmp));

    act = menu->addAction(tr("ST Sp&here"));
    act->setCheckable(true);
    act->setChecked(false);
    model_tmp = new Sphere(500,500);
    model_tmp->setModelId(models.size());
    models.append(qMakePair(act, model_tmp));

    act = menu->addAction(tr("ST P&lane"));
    act->setCheckable(true);
    act->setChecked(false);
    model_tmp = new Plane(500,500);
    model_tmp->setModelId(models.size());
    models.append(qMakePair(act, model_tmp));

    act = menu->addAction(tr("&Cube"));
    act->setCheckable(true);
    act->setChecked(false);
    model_tmp = new Cube();
    model_tmp->setModelId(models.size());
    models.append(qMakePair(act, model_tmp));

    act = menu->addAction(tr("&Tetrahedron"));
    act->setCheckable(true);
    act->setChecked(false);
    model_tmp = new Tetrahedron();
    model_tmp->setModelId(models.size());
    models.append(qMakePair(act, model_tmp));

    act = menu->addAction(tr("&Point"));
    act->setCheckable(true);
    act->setChecked(false);
    model_tmp = new Point();
    model_tmp->setModelId(models.size());
    models.append(qMakePair(act, model_tmp));

    act = menu->addAction(tr("T&rianglePatch"));
    act->setCheckable(true);
    act->setChecked(false);
    model_tmp = new PointPatch();
    model_tmp->setModelId(models.size());
    models.append(qMakePair(act, model_tmp));

    act = menu->addAction(tr("&IcosahedronPatch"));
    act->setCheckable(true);
    act->setChecked(false);
    model_tmp = new IcosahedronPatch();
    model_tmp->setModelId(models.size());
    models.append(qMakePair(act, model_tmp));



    connect(menu, SIGNAL(triggered(QAction*)),
            this, SLOT(modelChanged(QAction*)));
}

void RenderController::lightRotationToggle(bool lt)
{
    lightRotation = lt;
}

int RenderController::getModelId(void)
{
    return selectedObject;
}

void RenderController::setModelById(int ind)
{
    if(ind < 0 || ind >= models.size())
        return;

    for(int i = 0; i < models.size(); ++i)
    {
        models[i].first->setChecked(false);
    }

    models[ind].first->setChecked(true);
    selectedObject = ind;
    scene->setObjectToCurrent(models[ind].second->copy());

    display->updateGL();
}

QVector3D RenderController::getLightPosition() const
{
    return light->getLightPosition();
}

void RenderController::newSLObject()
{
    scene->addSLObject(objectController->newObject(models[selectedObject].second->copy()));
}

EditorController* RenderController::setShader( ShaderLab::Shader shaderType, const QString& filePath)
{
    SLShaderProgramController * pc = objectController->programController();
    return pc->setShader(shaderType,filePath);
}

void RenderController::setTexturesFromProject(const QStringList& list)
{
    objectController->setTexturesFromProject(list);
}

bool RenderController::closeAllFiles()
{
    SLShaderProgramController * pc = objectController->programController();
    return pc->closeAllShaders();
}

void RenderController::saveProjectAs(void)
{
    QString projectName = scene->currentSLObject()->saveMerge(true);
    if(!projectName.isEmpty())
        mainWindow->setSecondTitle(projectName);
}

void RenderController::saveProject(void)
{
    QString projectName = objectController->saveProject(false);
    if(!projectName.isEmpty())
        mainWindow->setSecondTitle(projectName);
}

void RenderController::setProject(Project* p)
{
    scene->currentSLObject()->setProject(p);
}

void RenderController::closeObject()
{
    saveProject();
    objectController->closeObject();
}

void RenderController::origin(bool v)
{
    scene->changeOrigin(v);
    display->updateGL();
}

void RenderController::objectsVisibility(bool v)
{
    scene->objectsVisibility(v);
    display->updateGL();
}
