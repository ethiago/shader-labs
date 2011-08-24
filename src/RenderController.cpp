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
    arcBall = new ArcBall(500);
    wireframe = false;

    this->display = new GLDisplay();
    mainWindow->setGLDisplay(display);

    {  // esta ordem deve ser mantida
    display->updateGL();

    configureModelsAndActions(mainWindow->modelsMenu());

    connect(display, SIGNAL(drawModel()), this ,SLOT(drawModel()));
    }

    this->primitivesDialog = new PrimitivesDialog(primitiveSetup(), mainWindow);

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

    connect(mainWindow->menuChangeOutputPrimitive(), SIGNAL(triggered()),
            this, SLOT(showPrimitiveSelector()));
}

RenderController::~RenderController()
{
    for(MMap::iterator it = models.begin(); it!= models.end(); ++it)
    {
        Object3D* obj = it.value();
        delete obj;
        obj = NULL;
    }
    models.clear();

    delete display;
    delete arcBall;
}

void RenderController::updateGL(void)
{
    display->updateGL();
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
    this->wireframe = wireframe;
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

void RenderController::modelChanged(QAction* action)
{
    MMap::iterator entry;
    entry = models.find(action);
    if(entry == models.end())
        return;

    for(MMap::iterator it = models.begin(); it != models.end(); ++it)
    {
        it.key()->setChecked(false);
    }

    entry.key()->setChecked(true);
    model = entry.value();
    model->cleanTransformations();

    display->updateGL();
}

void RenderController::configureModelsAndActions(QMenu* menu)
{
    QAction* act;

    act = menu->addAction(tr("&Sphere"));
    act->setCheckable(true);
    models[act] = new Sphere(50,50);
    act->setChecked(true);          // comeca pela esfera
    model = models[act];            // comeca pela esfera


    act = menu->addAction(tr("&Plane"));
    act->setCheckable(true);
    act->setChecked(false);
    models[act] = new Plane(50,50);

    act = menu->addAction(tr("ST Sp&here"));
    act->setCheckable(true);
    act->setChecked(false);
    models[act] = new Sphere(500,500);

    act = menu->addAction(tr("ST P&lane"));
    act->setCheckable(true);
    act->setChecked(false);
    models[act] = new Plane(500,500);

    connect(menu, SIGNAL(triggered(QAction*)),
            this, SLOT(modelChanged(QAction*)));
}

QStringList RenderController::primitiveSetup(void)
{
    QStringList text;

    primitives.append(GL_POINTS);
    primitives.append(GL_LINES);
    primitives.append(GL_LINE_STRIP);
    primitives.append(GL_LINE_LOOP);
    primitives.append(GL_TRIANGLES);
    primitives.append(GL_TRIANGLE_STRIP);
    primitives.append(GL_TRIANGLE_FAN);
    primitives.append(GL_QUADS);
    primitives.append(GL_QUAD_STRIP);
    primitives.append(GL_POLYGON);

    text.append("GL_POINTS");
    text.append("GL_LINES");
    text.append("GL_LINE_STRIP");
    text.append("GL_LINE_LOOP");
    text.append("GL_TRIANGLES");
    text.append("GL_TRIANGLE_STRIP");
    text.append("GL_TRIANGLE_FAN");
    text.append("GL_QUADS");
    text.append("GL_QUAD_STRIP");
    text.append("GL_POLYGON");

    return text;

}

void RenderController::showPrimitiveSelector(void)
{
    this->primitivesDialog->show();
}

int RenderController::getCurrentOutputPrimitive(void)
{
    return primitives[primitivesDialog->getCurrentOutputPrimitiveIndex()];
}

int RenderController::getCurrentInputPrimitive(void)
{
    return primitives[primitivesDialog->getCurrentInputPrimitiveIndex()];
}
