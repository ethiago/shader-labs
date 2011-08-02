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
#include "Arcball.h"

RenderController::RenderController(MainWindow *mainWindow,
                                   QObject *parent):
    QObject(parent)
{

    QMenu* menu = mainWindow->modelsMenu();

    actionSphere = menu->addAction(tr("&Sphere"));
    actionPlane = menu->addAction(tr("&Plane"));
    actionSphereST = menu->addAction(tr("ST Sp&here"));
    actionPlaneST = menu->addAction(tr("ST P&lane"));

    actionSphere->setCheckable(true);
    actionPlane->setCheckable(true);
    actionSphereST->setCheckable(true);
    actionPlaneST->setCheckable(true);

    arcBall = new ArcBall(500);

    this->display = new GLDisplay();
    mainWindow->setGLDisplay(display);

    display->updateGL(); // cria o contexto openGL

    model = new Sphere();
    actionSphere->setChecked(true);
    actionPlane->setChecked(false);
    actionSphereST->setChecked(false);
    actionPlaneST->setChecked(false);

    connect(display, SIGNAL(drawModel()), this ,SLOT(drawModel()));

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

    connect(menu, SIGNAL(triggered(QAction*)),
            this, SLOT(modelChanged(QAction*)));
}

RenderController::~RenderController()
{
    delete display;
    delete arcBall;
    delete model;
}

void RenderController::updateGL(void)
{
    display->updateGL();
}

void RenderController::drawModel(void)
{
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
    model->addRotacao(t);
    model->setInteractiveQuartenion(QQuaternion());
    display->updateGL();
}

void RenderController::mouseCancel()
{
    model->setTraslation(QVector3D());
    model->setInteractiveQuartenion(QQuaternion());
    display->updateGL();
}

void RenderController::wireFrameToggle(bool wireframe)
{
    display->setWireframe(wireframe);
    display->updateGL();
}

void RenderController::saveResultAsImage()
{
    QString filePath = QFileDialog::getSaveFileName(display, "Save Result As ...", "/home", "*.png");

    if(filePath.isEmpty())
        return;

    display->updateGL();
    display->grabFrameBuffer().save(filePath);
}

QGLWidget* RenderController::getGLContext(void)
{
    return display;
}

void RenderController::updateTexture(int texName)
{
    model->setTexture(texName);
}

void RenderController::modelChanged(QAction* action)
{
    if(action != actionSphere && action != actionPlane && action != actionSphereST && action != actionPlaneST)
        return;

    delete model;
    if(action == actionSphere)
    {
        actionSphere->setChecked(true);
        actionPlane->setChecked(false);
        actionSphereST->setChecked(false);
        actionPlaneST->setChecked(false);
        model = new Sphere();
    }else if(action == actionPlane)
    {
        actionSphere->setChecked(false);
        actionPlane->setChecked(true);
        actionSphereST->setChecked(false);
        actionPlaneST->setChecked(false);
        model = new Plane(50, 50);
    }else if(action == actionSphereST)
    {
        actionSphere->setChecked(false);
        actionPlane->setChecked(false);
        actionSphereST->setChecked(true);
        actionPlaneST->setChecked(false);
        model = new Sphere(500,500);
    }else if(action == actionPlaneST)
    {
        actionSphere->setChecked(false);
        actionPlane->setChecked(false);
        actionSphereST->setChecked(false);
        actionPlaneST->setChecked(true);
        model = new Plane(500, 500);
    }
    display->updateGL();
}
