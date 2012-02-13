#include <QToolTip>
#include <QDebug>
#include <QtOpenGL>
#include <QPoint>

#include "RenderController.h"
#include "GLDisplay.h"
#include "MainWindow.h"
#include "Object3D.h"
#include "Sphere.h"
#include "Plane.h"
#include "Cube.h"
#include "Point.h"
#include "Scene3D.h"
#include "Tetrahedron.h"
#include "Arcball.h"
#include "DirectionalLight.h"
#include "PrimitivesDialog.h"
#include "GlobalProperties.h"
#include "InterfaceRequests.h"

RenderController::RenderController(MainWindow *mainWindow,
                                   QObject *parent):
    QObject(parent)
{
    scene = new Scene3D();
    light = new DirectionalLight;
    arcBall = new ArcBall(500);
    lightRotation = false;


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

    ShaderLab *sl = ShaderLab::instance();
    this->primitivesDialog = new PrimitivesDialog(outPrimitiveSetup(),  mainWindow);
    if(!sl->geometryShaderEnabled())
    {
        mainWindow->setEnableMenuGeometryShader(false);
    }
    else
    {
        mainWindow->setEnableMenuGeometryShader(true);

        connect(mainWindow->menuChangeOutputPrimitive(), SIGNAL(triggered()),
            this, SLOT(showPrimitiveSelector()));
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

}

RenderController::~RenderController()
{
    scene->clearObjects();
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
    model->setWireframe(!model->wireframe());
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

QGLWidget* RenderController::getGLContext(void)
{
    return display;
}

void RenderController::modelChanged(QAction* action)
{
    for(int i = 0; i < models.size(); ++i)
    {
        models[i].first->setChecked(false);
        if(models[i].first == action)
        {
           models[i].first->setChecked(true);
           model = models[i].second;
           scene->clearObjects();
           scene->addObject(models[i].second);
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
    model = model_tmp;            // comeca pela esfera
    scene->addObject(model_tmp);
    models.append(qMakePair(act, model_tmp));

    act = menu->addAction(tr("&Plane"));
    act->setCheckable(true);
    act->setChecked(false);
    model_tmp = new Plane(50,50);
    models.append(qMakePair(act, model_tmp));

    act = menu->addAction(tr("ST Sp&here"));
    act->setCheckable(true);
    act->setChecked(false);
    model_tmp = new Sphere(500,500);
    models.append(qMakePair(act, model_tmp));

    act = menu->addAction(tr("ST P&lane"));
    act->setCheckable(true);
    act->setChecked(false);
    model_tmp = new Plane(500,500);
    models.append(qMakePair(act, model_tmp));

    act = menu->addAction(tr("&Cube"));
    act->setCheckable(true);
    act->setChecked(false);
    model_tmp = new Cube();
    models.append(qMakePair(act, model_tmp));

    act = menu->addAction(tr("&Tetrahedron"));
    act->setCheckable(true);
    act->setChecked(false);
    model_tmp = new Tetrahedron();
    models.append(qMakePair(act, model_tmp));

    act = menu->addAction(tr("&Point"));
    act->setCheckable(true);
    act->setChecked(false);
    model_tmp = new Point();
    models.append(qMakePair(act, model_tmp));

    connect(menu, SIGNAL(triggered(QAction*)),
            this, SLOT(modelChanged(QAction*)));
}

QStringList RenderController::outPrimitiveSetup(void)
{
    QStringList text;

    primitives.append(GL_POINTS);
    primitives.append(GL_LINE_STRIP);
    primitives.append(GL_TRIANGLE_STRIP);


    text.append("GL_POINTS");
    text.append("GL_LINE_STRIP");
    text.append("GL_TRIANGLE_STRIP");

    return text;
}

void RenderController::showPrimitiveSelector(void)
{
    this->primitivesDialog->show();
}

GLenum RenderController::getCurrentOutputPrimitive(void)
{
    return primitives[primitivesDialog->getCurrentOutputPrimitiveIndex()];
}

GLenum RenderController::getCurrentInputPrimitive(void)
{
    //return primitives[primitivesDialog->getCurrentInputPrimitiveIndex()];
    return model->inputType();

}

void RenderController::lightRotationToggle(bool lt)
{
    lightRotation = lt;
}

int RenderController::getModelId(void)
{
    for(int i = 0; i < models.size(); ++i)
    {
        if(models[i].second == model)
            return i;
    }

    return -1;
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
    model = models[ind].second;
    scene->clearObjects();
    scene->addObject(models[ind].second);

    display->updateGL();
}

QVector3D RenderController::getLightPosition() const
{
    return light->getLightPosition();
}
