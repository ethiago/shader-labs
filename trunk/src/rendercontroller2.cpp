#include "gl3w.h"
#include "rendercontroller2.h"
#include "GLDisplay.h"
#include "SLObject.h"
#include "MainWindow.h"
#include "Object3D.h"
#include "Sphere.h"
#include "Plane.h"
#include "Cube.h"
#include "Point.h"
#include "squarepatch.h"
#include "Scene3D.h"
#include "Tetrahedron.h"
#include "TrianglePatch.h"
#include "IcosahedronPatch.h"
#include "Arcball.h"
#include "DirectionalLight.h"
#include "slshadercontroller.h"
#include "slTextureController.h"
#include "Project.h"
#include "plyio.h"
#include "meshextraction.h"
//#include "slMesh.h"
//#include "slMeshContainer.h"
#include "slhemesh.h"
#include "slhemeshconstructor.h"
#include "SLFile.h"

#include "GlobalProperties.h"
#include "InterfaceRequests.h"
#include "objectcontroller.h"

#include <QToolTip>
#include <QDebug>
#include <QtOpenGL>
#include <QPoint>
#include <QAction>

RenderController2::RenderController2(MainWindow *mainWindow, SLShaderController * shaderController,
                                   SLTextureController * texture):
    QObject(NULL)
{
    e_shaderController = shaderController;
    e_textureController = texture;
    e_mainwindow = mainWindow;

    m_scene = new Scene3D();
    m_light = new DirectionalLight;
    m_arcBall = new ArcBall(500);
    lightRotation = false;
    selectedObject = -1;

    this->m_display = new GLDisplay(new QGLContext(QGLFormat::defaultFormat()));
    mainWindow->setGLDisplay(m_display);

    {  // esta ordem deve ser mantida
        m_display->updateGL();

        configureModelsAndActions(mainWindow->modelsMenu());

        connect(m_display, SIGNAL(drawModel()),
                this, SLOT(drawModel()));

        connect(m_display, SIGNAL(lightSetup()),
                this, SLOT(lightSetup()));
    }


    { // configuracao das propriedades globais
        m_propertries = new GlobalProperties(e_mainwindow);
        m_propertries->setWidget(m_display->getPropertyBrowser());
        e_mainwindow->addDockWidget(Qt::RightDockWidgetArea, m_propertries);
        m_propertries->close();
    }

    m_loadModel = new QAction(QString("Load Model ..."), NULL);
    m_loadModel->setShortcut(QKeySequence::fromString("Ctrl+L"));
    e_mainwindow->menuViewInsertAction(m_loadModel);

    connect(m_display, SIGNAL(mouseLefthFinish(QPoint,QPoint)),
            this, SLOT(mouseLefthFinish(QPoint,QPoint)));

    connect(m_display, SIGNAL(mouseLeftMove(QPoint,QPoint)),
            this, SLOT(mouseLeftMove(QPoint,QPoint)));

    connect(m_display, SIGNAL(mouseRigthFinish(QPoint,QPoint)),
            this, SLOT(mouseRigthFinish(QPoint,QPoint)));

    connect(m_display, SIGNAL(mouseRigthMove(QPoint,QPoint)),
            this, SLOT(mouseRigthMove(QPoint,QPoint)));

    connect(m_display, SIGNAL(mouseCancel()),
            this, SLOT(mouseCancel()));

    connect(e_mainwindow, SIGNAL(wireframeClicked()),
            this, SLOT(wireFrameToggle()));

    connect(e_mainwindow, SIGNAL(saveResultAsImage()),
            this, SLOT(saveResultAsImage()));

    connect(e_mainwindow, SIGNAL(lightRotationToggle(bool)),
            this, SLOT(lightRotationToggle(bool)));

    connect(e_mainwindow, SIGNAL(origin(bool)),
            this, SLOT(origin(bool)));

    connect(e_mainwindow, SIGNAL(objectsVisibility(bool)),
            this, SLOT(objectsVisibility(bool)));

    connect(m_loadModel, SIGNAL(triggered()),
            this, SLOT(loadModel()) );

    Object3D *obj = m_models[selectedObject].second;
    SLObject *slobj = new SLObject(obj);
    e_shaderController->setShaderObjects(slobj->shaderProgram(), slobj->shaderCodes());
    e_shaderController->setInputPrimitive(obj->inputType());
    e_textureController->setTextures(slobj->textures());
    m_scene->addSLObject(slobj);
    m_objectController = new ObjectController(e_mainwindow, slobj);

    connect(m_objectController, SIGNAL(objectChanged(int)),
            this, SLOT(objectChanged(int)));
}

RenderController2::~RenderController2()
{
    for(int i = 0 ; i < m_models.size(); ++i)
    {
        Object3D* obj = m_models[i].second;
        delete obj;
    }
    m_models.clear();

    delete m_display;
    delete m_arcBall;
    delete m_light;
    delete m_propertries;
    delete m_scene;
    delete m_objectController;
}

void RenderController2::changeCurrent(Object3D* obj)
{
    SLObject * current = m_scene->currentSLObject();
    if(current != NULL)
    {
        current->changeObject(obj);
        e_shaderController->setInputPrimitive(obj->inputType());
        m_objectController->updateView();
    }
}

Scene3D * RenderController2::getScene()
{
    return m_scene;
}

void RenderController2::updateGL(void)
{
    m_display->updateGL();
}

void RenderController2::lightSetup(void)
{
    m_light->draw();
}

void RenderController2::drawModel(void)
{
    m_scene->draw();
}

void RenderController2::mouseRigthMove(QPoint begin, QPoint curr)
{
    QVector3D v(curr-begin);
    m_scene->setTraslation(v/40.0);
    m_display->updateGL();
}

void RenderController2::mouseRigthFinish(QPoint ini, QPoint curr)
{
    QVector3D v(curr-ini);
    m_scene->setTraslation(QVector3D());
    m_scene->setCenter(m_scene->center() + (v/40.0));
    m_display->updateGL();
}

void RenderController2::mouseLeftMove(QPoint ini, QPoint curr)
{
    if(ini == curr)
        return;
    QPoint center(m_display->width()/2, m_display->height()/2);

    if(lightRotation)
        m_light->setInteractiveQuartenion(
                m_arcBall->rotationForPoints(center,curr, ini));
    else
        m_scene->setInteractiveQuartenion(
                m_arcBall->rotationForPoints(center,curr, ini));

    m_display->updateGL();
}

void RenderController2::mouseLefthFinish(QPoint ini, QPoint curr)
{
    if(ini == curr)
        return;
    QPoint center(m_display->width()/2, m_display->height()/2);
    QQuaternion t = m_arcBall->rotationForPoints(center,curr, ini);
    if(lightRotation)
    {
        m_light->addRotation(t);
        m_light->setInteractiveQuartenion(QQuaternion());
    }else
    {
        m_scene->addRotation(t);
        m_scene->setInteractiveQuartenion(QQuaternion());
    }

    m_display->updateGL();
}

void RenderController2::mouseCancel()
{
    if(lightRotation)
        m_light->setInteractiveQuartenion(QQuaternion());
    else
        m_scene->setInteractiveQuartenion(QQuaternion());

    m_display->updateGL();
}

void RenderController2::wireFrameToggle()
{
    SLObject * current = m_scene->currentSLObject();
    current->toggleWireframe();
    m_display->updateGL();
}

void RenderController2::saveResultAsImage()
{
    QString filePath = InterfaceRequests::saveImage();

    if(filePath.isEmpty())
        return;

    m_display->updateGL();
    m_display->grabFrameBuffer().save(filePath);
}

void RenderController2::modelChanged(QAction* action)
{
    for(int i = 0; i < m_models.size(); ++i)
    {
        m_models[i].first->setChecked(false);
        if(m_models[i].first == action)
        {
           m_models[i].first->setChecked(true);
           changeCurrent(m_models[i].second);
           selectedObject = i;
        }
    }

    m_display->updateGL();
}

void RenderController2::loadModel()
{
    QString fn = InterfaceRequests::loadModel();

    if(fn.isEmpty())
        return;

    PLYIO plyio;

    if(!plyio.load(fn))
        return;

    SLHEMeshConstructor constructor;
    MeshExtraction extractor(plyio.getData(), &constructor);

    if(!extractor.extract())
        return;

    QMenu* menu = e_mainwindow->modelsMenu();

    QAction* act = NULL;
    SLHEMesh *model_tmp = constructor.getObject();

    connect(e_shaderController, SIGNAL(afterLink(GLuint)),
            m_objectController, SLOT(afterLink(GLuint)) );

    model_tmp->storeList();

    act = menu->addAction( SLFile::fileNameWithoutExt(fn) );
    act->setCheckable(true);
    act->setChecked(false);
    model_tmp->setModelId(m_models.size());
    m_models.append(qMakePair(act, (Object3D*)model_tmp));
    modelChanged(act);
}

void RenderController2::configureModelsAndActions(QMenu* menu)
{
    QAction* act;
    Object3D *model_tmp;

    act = menu->addAction(tr("&Sphere"));
    act->setCheckable(true);
    model_tmp = new Sphere(50,50);
    act->setChecked(true);          // comeca pela esfera
    model_tmp->setModelId(m_models.size());
    selectedObject = 0;
    changeCurrent(model_tmp);
    m_models.append(qMakePair(act, model_tmp));

    act = menu->addAction(tr("&Plane"));
    act->setCheckable(true);
    act->setChecked(false);
    model_tmp = new Plane(50,50);
    model_tmp->setModelId(m_models.size());
    m_models.append(qMakePair(act, model_tmp));

    act = menu->addAction(tr("ST Sp&here"));
    act->setCheckable(true);
    act->setChecked(false);
    model_tmp = new Sphere(500,500);
    model_tmp->setModelId(m_models.size());
    m_models.append(qMakePair(act, model_tmp));

    act = menu->addAction(tr("ST P&lane"));
    act->setCheckable(true);
    act->setChecked(false);
    model_tmp = new Plane(500,500);
    model_tmp->setModelId(m_models.size());
    m_models.append(qMakePair(act, model_tmp));

    act = menu->addAction(tr("&Cube"));
    act->setCheckable(true);
    act->setChecked(false);
    model_tmp = new Cube();
    model_tmp->setModelId(m_models.size());
    m_models.append(qMakePair(act, model_tmp));

    act = menu->addAction(tr("&Tetrahedron"));
    act->setCheckable(true);
    act->setChecked(false);
    model_tmp = new Tetrahedron();
    model_tmp->setModelId(m_models.size());
    m_models.append(qMakePair(act, model_tmp));

    act = menu->addAction(tr("&Point"));
    act->setCheckable(true);
    act->setChecked(false);
    model_tmp = new Point();
    model_tmp->setModelId(m_models.size());
    m_models.append(qMakePair(act, model_tmp));

    act = menu->addAction(tr("T&rianglePatch"));
    act->setCheckable(true);
    act->setChecked(false);
    model_tmp = new TrianglePatch();
    model_tmp->setModelId(m_models.size());
    m_models.append(qMakePair(act, model_tmp));

    act = menu->addAction(tr("&IcosahedronPatch"));
    act->setCheckable(true);
    act->setChecked(false);
    model_tmp = new IcosahedronPatch();
    model_tmp->setModelId(m_models.size());
    m_models.append(qMakePair(act, model_tmp));

    act = menu->addAction(tr("S&quare"));
    act->setCheckable(true);
    act->setChecked(false);
    model_tmp = new Plane(1,1);
    model_tmp->setModelId(m_models.size());
    m_models.append(qMakePair(act, model_tmp));

    act = menu->addAction(tr("SquarePatch"));
    act->setCheckable(true);
    act->setChecked(false);
    model_tmp = new SquarePatch();
    model_tmp->setModelId(m_models.size());
    m_models.append(qMakePair(act, model_tmp));

    connect(menu, SIGNAL(triggered(QAction*)),
            this, SLOT(modelChanged(QAction*)));
}

void RenderController2::lightRotationToggle(bool lt)
{
    lightRotation = lt;
    m_light->setShowLine(lt);
    m_display->updateGL();
}

int RenderController2::getModelId(void)
{
    return selectedObject;
}

void RenderController2::setModelById(int ind)
{
    if(ind < 0 || ind >= m_models.size())
        return;

    for(int i = 0; i < m_models.size(); ++i)
    {
        m_models[i].first->setChecked(false);
    }

    m_models[ind].first->setChecked(true);
    selectedObject = ind;
    changeCurrent(m_models[ind].second);

    m_display->updateGL();
}

QVector3D RenderController2::getLightPosition() const
{
    return m_light->getLightPosition();
}

bool RenderController2::newSLObject()
{
    if(e_shaderController->canChangeObject())
    {
        SLObject *obj = new SLObject(m_models[selectedObject].second);
        e_shaderController->setShaderObjects(obj->shaderProgram(), obj->shaderCodes());
        e_textureController->setTextures(obj->textures());
        m_scene->addSLObject(obj);
        m_objectController->newObject(obj);
        return true;
    }else
    {
        return false;
    }
}

void RenderController2::origin(bool v)
{
    m_scene->changeOrigin(v);
    m_display->updateGL();
}

void RenderController2::objectsVisibility(bool v)
{
    m_scene->objectsVisibility(!v);
    m_display->updateGL();
}

void RenderController2::projectOpened(Project* p)
{
    setModelById(p->getModelId());
}

void RenderController2::objectChanged(int idx)
{
    SLObject *obj = m_scene->changeCurrent(idx);
    if(obj == NULL)
        return;

    e_shaderController->setShaderObjects(obj->shaderProgram(), obj->shaderCodes());
    e_textureController->setTextures(obj->textures());
    m_objectController->setObject(obj);

    setModelById(obj->modelId());

    emit objectChanged();

    m_display->updateGL();
}
