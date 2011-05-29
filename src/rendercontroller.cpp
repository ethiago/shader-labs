#include "rendercontroller.h"
#include "gldisplay.h"
#include "mainwindow.h"

RenderController::RenderController(const MainWindow* mainWindow,
                                            QObject *parent):
    QObject(parent)
{
    //connect(display, SIGNAL(drawModel()), this ,SIGNAL(drawModel()));
}
