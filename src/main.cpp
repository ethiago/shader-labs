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

    return app.exec();
}
