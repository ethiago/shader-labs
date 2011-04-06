#include <QString>
#include <QFile>

#include "filecontroller.h"

FileController::FileController(QString filepath, ShaderLab::Shader shadertype,
                               QObject *parent) : QObject(parent)
{
    filePath = filepath;
    shaderType = shadertype;

    switch(shadertype)
    {
        case ShaderLab::Vertex:
            shader = new QGLShader(QGLShader::Vertex, this);
            break;
        case ShaderLab::Fragment:
            shader = new QGLShader(QGLShader::Fragment, this);
            break;
    }
}

FileController::~FileController()
{
    delete shader;
}

QString FileController::getFileContent(void)
{
    QString content;
    QFile file(filePath);

    if(file.open(QFile::ReadOnly))
    {
        content = file.readAll();
        file.close();
        return content;
    }

    return QString();
}

int FileController::close(void)
{
    return 1;
}

bool FileController::compile(const QString& code)
{
    if(!code.isEmpty())
    {
        return shader->compileSourceCode(code);
    }

    return false;
}

bool FileController::compile(void)
{
    return shader->compileSourceFile(filePath);
}

QString FileController::log()
{
    return shader->log();
}

QGLShader *FileController::getShader(void)
{
    return shader;
}
