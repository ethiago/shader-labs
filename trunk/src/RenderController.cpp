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
    light = new DirectionalLight;
    arcBall = new ArcBall(500);
    wireframe = false;
    lightRotation = false;

    this->display = new GLDisplay();
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
    this->primitivesDialog = new PrimitivesDialog(inPrimitiveSetup(), outPrimitiveSetup(),  mainWindow);
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

    connect(mainWindow, SIGNAL(wireframeClicked(bool)),
            this, SLOT(wireFrameToggle(bool)));

    connect(mainWindow, SIGNAL(saveResultAsImage()),
            this, SLOT(saveResultAsImage()));

    connect(mainWindow, SIGNAL(lightRotationToggle(bool)),
            this, SLOT(lightRotationToggle(bool)));

}

RenderController::~RenderController()
{
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
    if(wireframe)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);

    model->draw();
}

void RenderController::mouseRigthMove(QPoint begin, QPoint curr)
{
    QVector3D v(curr-begin);
    model->setTraslation(v/40.0);
    display->updateGL();
}

void RenderController::mouseRigthFinish(QPoint ini, QPoint curr)
{
    QVector3D v(curr-ini);
    model->setTraslation(QVector3D());
    model->setCenter(model->center() + (v/40.0));
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
        model->setInteractiveQuartenion(
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
        model->addRotation(t);
        model->setInteractiveQuartenion(QQuaternion());
    }

    display->updateGL();
}

void RenderController::mouseCancel()
{
    if(lightRotation)
        light->setInteractiveQuartenion(QQuaternion());
    else
        model->setInteractiveQuartenion(QQuaternion());

    display->updateGL();
}

void RenderController::wireFrameToggle(bool wireframe)
{
    this->wireframe = wireframe;
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
           model->cleanTransformations();
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

    connect(menu, SIGNAL(triggered(QAction*)),
            this, SLOT(modelChanged(QAction*)));
}

QStringList RenderController::inPrimitiveSetup(void)
{
    QStringList text;

    primitives.append(GL_POINTS);
    primitives.append(GL_LINES);
    primitives.append(GL_LINES_ADJACENCY_EXT);
    primitives.append(GL_TRIANGLES);
    primitives.append(GL_TRIANGLES_ADJACENCY_EXT);


    text.append("GL_POINTS");
    text.append("GL_LINES");
    text.append("GL_LINES_ADJACENCY_EXT");
    text.append("GL_TRIANGLES");
    text.append("GL_TRIANGLES_ADJACENCY_EXT");

    return text;
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
    return primitives[primitivesDialog->getCurrentInputPrimitiveIndex()];
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
    model->cleanTransformations();

    display->updateGL();
}

QVector3D RenderController::getLightPosition() const
{
    return light->getLightPosition();
}
