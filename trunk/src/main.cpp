#include <QtGui/QApplication>
#include "mainwindow.h"
#include "filecontroller.h"
#include "shadercodecontroller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    FileController *fc = new FileController(&w);
    ShaderCodeController *scc = new ShaderCodeController(fc, &w);
    w.show();

    return a.exec();
}
