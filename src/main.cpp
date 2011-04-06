#include <QtGui/QApplication>

#include "mainwindow.h"
#include "filecontroller.h"
#include "maincontroller.h"

int main(int argc, char *argv[])
{
    QApplication a(argc, argv);
    new MainController();

    return a.exec();
}
