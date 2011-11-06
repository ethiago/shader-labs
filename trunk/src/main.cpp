#include <QtGui/QApplication>

#include "TextureController.h"
#include "MainController.h"
#include "RenderController.h"
#include "MainWindow.h"
#include "GLDisplay.h"
#include "Global.h"
#include <QDebug>
#include <QMessageBox>




int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow mw;
    MainController mc(&mw);

    if(argc == 2)
    {
        QString fn(argv[1]);

        if(fn.right(3) == "slp")
            mc.openProject(fn);

    }

    return app.exec();
}
