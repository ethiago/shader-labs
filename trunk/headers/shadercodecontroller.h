#ifndef SHADERCODECONTROLLER_H
#define SHADERCODECONTROLLER_H

#include <QObject>
#include "global.h"

class FileController;
class MainWindow;

class ShaderCodeController : public QObject
{
    Q_OBJECT

public:
    explicit ShaderCodeController(FileController* fc, MainWindow *w);

signals:

public slots:
    void runShaderCode(void);

private:
    FileController *fileController;
    MainWindow *mainWindow;

};

#endif // SHADERCODECONTROLLER_H
