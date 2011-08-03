#include <QtGui/QApplication>

#include "TextureController.h"
#include "MainController.h"
#include "RenderController.h"
#include "MainWindow.h"
#include "GLDisplay.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow mw;
    MainController mc(&mw);
    RenderController rc(&mw);
    TextureController tc(&mw, rc.getGLContext());

    mc.setTextureController(&tc);

    QObject::connect(&mc, SIGNAL(updateGL()), &rc, SLOT(updateGL()));

    return app.exec();
}
