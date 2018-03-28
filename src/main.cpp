#include "SLVersion.h"
#include <iostream>
#include <QtGui/QApplication>
#include "MainController.h"
#include "MainWindow.h"

int main(int argc, char *argv[])
{
    if(argc > 1)
    {
        if ((!strcmp(argv[1],"-h")||!strcmp(argv[1],"--help")))
        {
            std::cout<<"Usage: "<<argv[0]<<" [option|slp file]"<<
                       "\nwhere option is one of\n"<<
                       "-h, --help\tprint this message and exit\n"<<
                       "-v, --version\tprint version information and exit\n"<<
                       "-l, --license\tshow information about the license"<<
                       std::endl;
            return 0;
        }
        if ((!strcmp(argv[1],"-v")||!strcmp(argv[1],"--version")))
        {
            std::cout<<"ShaderLabs "<<
                       SL_VERSION_MAJOR<<'.'<<
                       SL_VERSION_MINOR<<'.'<<
                       SL_VERSION_REV<<std::endl;
            return 0;
        }
        if ((!strcmp(argv[1],"-l")||!strcmp(argv[1],"--license")))
        {
            std::cout<<
                        "ShaderLabs "<<
                        SL_VERSION_MAJOR<<'.'<<
                        SL_VERSION_MINOR<<'.'<<
                        SL_VERSION_REV<<
                        "\nCopyright (C) 2011 Thiago E. Gomes & F. V. Vianna, "<<
                        "2012-2014 Thiago E. Gomes\n"<<
                        "This is free software; see the source for copying "<<
                        "conditions.  There is NO\nwarranty; not even for "<<
                        "MERCHANTABILITY or FITNESS FOR A PARTICULAR PURPOSE."<<
                        std::endl;
            return 0;
        }
    }

    QApplication app(argc, argv);

    MainWindow mw;
    MainController mc(&mw);

    if ((!strcmp(argv[1],"-c")||!strcmp(argv[1],"--check")))
    {
        return 0;
    }

    return app.exec();
}
