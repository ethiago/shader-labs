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
    ~MainController();

signals:

public slots:
    void openShaderCode(const QString& filename, ShaderLab::Shader);
    void newShaderCode(ShaderLab::Shader);
    void closeShaderCode(ShaderLab::Shader);
    void runAllActiveShaders(void);
    void programCloseRequest(void);
    void saveFile(ShaderLab::Shader shadertype);
    void saveFileAs(ShaderLab::Shader shadertype, const QString&, const QString&);
    void fileChanged(ShaderLab::Shader);
    void saveAll();

private:
    MainWindow *mainWindow;
    QMap<ShaderLab::Shader, FileController*> fileControllers;
    void saveLogic(FileController*);

};

#endif // MAINCONTROLLER_H