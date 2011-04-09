#ifndef FILECONTROLLER_H
#define FILECONTROLLER_H

#include <QObject>
#include <QGLShader>

#include "mainwindow.h"
#include "global.h"

class FileController : public QObject
{
    Q_OBJECT

public:
    explicit FileController(QString filepath, ShaderLab::Shader shadertype,
                            QObject *parent = 0);

    ~FileController();

    QString getFileContent(void);

    bool compile(const QString& code);
    bool compile(void);
    QGLShader* getShader(void);
    QString log();

private:
    QString filePath;
    QGLShader *shader;
    ShaderLab::Shader shaderType;

};

#endif // FILECONTROLLER_H
