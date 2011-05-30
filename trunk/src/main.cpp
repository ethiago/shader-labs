#include <QtGui/QApplication>

#include "maincontroller.h"
#include "rendercontroller.h"
#include "mainwindow.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

/*
    MainWindow *mw = new MainWindow();
    MainController *mc = new MainController(mw);
    RenderController *rc = new RenderController(mw, mc);
    QObject::connect(mc, SIGNAL(updateGL()), rc, SLOT(updateGL()));

    int ret = a.exec();

    delete mw;
    delete mc;
    delete rc;

    return ret;
//*/
//*
    MainWindow mw;
    MainController mc(&mw);
    RenderController rc(&mw, &mc);
    QObject::connect(&mc, SIGNAL(updateGL()), &rc, SLOT(updateGL()));

    return a.exec();
//*/

}
