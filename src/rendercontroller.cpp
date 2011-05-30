#include <QToolTip>
#include <QDebug>

#include "rendercontroller.h"
#include "gldisplay.h"
#include "mainwindow.h"
#include "object3d.h"
#include "sphere.h"

RenderController::RenderController(MainWindow *mainWindow,
                                            QObject *parent):
    QObject(parent)
{
    display = new GLDisplay();
    model = new Sphere();
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

void RenderController::mouseRigthMove(QPoint ini, QPoint curr)
{
    QVector3D v(curr.x() - ini.x(), ini.y() - curr.y(), 0);
    model->setTraslation(v/40.0);
    display->updateGL();
}

void RenderController::mouseRigthFinish(QPoint ini, QPoint curr)
{
    QVector3D v(curr.x() - ini.x(), ini.y() - curr.y(), 0);
    model->setTraslation(QVector3D());
    model->setCenter(v/40.0);
    display->updateGL();
}

void RenderController::mouseLeftMove(QPoint ini, QPoint curr)
{
    QToolTip::showText(ini, "ArcBall");
}

void RenderController::mouseLefthFinish(QPoint ini, QPoint curr)
{
    QToolTip::showText(ini, "ArcBall_FIM");
}

void RenderController::mouseCancel()
{
    model->setTraslation(QVector3D());
    display->updateGL();
}
