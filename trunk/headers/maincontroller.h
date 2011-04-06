#ifndef MAINCONTROLLER_H
#define MAINCONTROLLER_H

#include <QObject>
#include <QMap>

#include "global.h"

class MainWindow;
class FileController;

class MainController : public QObject
{
Q_OBJECT
public:
    explicit MainController(QObject *parent = 0);

signals:

public slots:
    void openShaderCode(const QString& filename, ShaderLab::Shader);
    void closeShaderCode(ShaderLab::Shader);
    void runAllActiveShaders(void);

private:
    MainWindow *mainWindow;
    QMap<ShaderLab::Shader, FileController*> fileControllers;

};

#endif // MAINCONTROLLER_H
