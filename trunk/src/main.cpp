#include "SLVersion.h"
#include <iostream>
#include <QtGui/QApplication>
#include <QFileInfo>
#include "MainController.h"
#include "MainWindow.h"
#include "Global.h"
#include <GL/glew.h>

int main(int argc, char *argv[])
{
    if(argc > 1)
    {
        if ((!strcmp(argv[1],"-v")||!strcmp(argv[1],"--version")))
        {
            std::cout<<"ShaderLabs "<<
                       SL_VERSION_MAJOR<<'.'<<
                       SL_VERSION_MINOR<<'.'<<
                       SL_VERSION_REV<<std::endl;
            exit(-1);
        }
        if ((!strcmp(argv[1],"-h")||!strcmp(argv[1],"--help")))
        {
            std::cout<<"Usage: "<<argv[0]<<" [option|slp file]"<<
                       "\nwhere option is one of\n"<<
                       "-h, --help\tprint this message and exit\n"<<
                       "-v, --version\tprint version information and exit"<<
                       std::endl;
            exit(-1);
        }
    }

    QApplication app(argc, argv);

    MainWindow mw;
    MainController mc(&mw);

    if(argc > 1)
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
