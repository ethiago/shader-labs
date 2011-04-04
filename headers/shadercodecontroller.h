#ifndef SHADERCODECONTROLLER_H
#define SHADERCODECONTROLLER_H

#include <QObject>
#include "global.h"

class FileController;

class ShaderCodeController : public QObject
{
    Q_OBJECT

public:
    explicit ShaderCodeController(FileController* fc);
    void runShaderCode(ShaderLab::Shader);

signals:

public slots:

private:
    FileController *fileController;

};

#endif // SHADERCODECONTROLLER_H
