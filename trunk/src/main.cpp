#include <QtGui/QApplication>

#include "maincontroller.h"
#include "rendercontroller.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow mw;
    MainController mc(&mw);
    RenderController rc(&mw, &mc);
    QObject::connect(&mc, SIGNAL(updateGL()), &rc, SLOT(updateGL()));

    return a.exec();


}
