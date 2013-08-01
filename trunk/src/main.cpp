#include <QtGui/QApplication>
#include <QFileInfo>
#include "MainController.h"
#include "MainWindow.h"
#include "Global.h"

int main(int argc, char *argv[])
{
    QApplication app(argc, argv);

    MainWindow mw;
    MainController mc(&mw);

    if(argc == 2)
    {
        QFileInfo fi(argv[1]);

        if(fi.isFile())
        {
            if(fi.suffix() == "slp")
            {
                mc.openProject(fi.absoluteFilePath());
            }
            else
            {
                mc.openFileFromArg(fi.absoluteFilePath());
            }
        }
    }

    return app.exec();
}
