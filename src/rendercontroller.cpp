#include <QToolTip>
#include <QDebug>
#include <QtOpenGL>
#include <QPoint>

#include "rendercontroller.h"
#include "GLDisplay.h"
#include "mainwindow.h"
#include "object3d.h"
#include "sphere.h"
#include "Plane.h"
#include "Arcball.h"

RenderController::RenderController(MainWindow *mainWindow,
                                            QObject *parent):
    QObject(parent)
{
    display = new GLDisplay();
    //model = new Sphere();
    model = new Plane();
    arcBall = new ArcBall(500);


    mainWindow->setGLDisplay(display);

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
}

RenderController::~RenderController()
{
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
    QString filePath = QFileDialog::getSaveFileName(display, "Save Result As ...", "", "*.png");

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
