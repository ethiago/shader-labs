#include <QtGui/QApplication>
#include "mainwindow.h"
#include "filecontroller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);

    MainWindow w;
    FileController *fc = new FileController(&w);
    w.show();

    return a.exec();
}
