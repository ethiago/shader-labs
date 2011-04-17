#include <QString>
#include <QFile>

#include "filecontroller.h"

FileController::FileController(QString filepath, ShaderLab::Shader shadertype,
                               QObject *parent) : QObject(parent)
{
    filePath = filepath;
    shaderType = shadertype;
    isNew = false;
    changed = false;

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

FileController::FileController(ShaderLab::Shader shadertype, QObject *parent) : QObject(parent)
{
    shaderType = shadertype;
    isNew = true;
    changed = true;

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
    QFile file(filePath.absoluteFilePath());

    if(file.open(QFile::ReadOnly))
    {
        content = file.readAll();
        file.close();
        return content;
    }

    return QString();
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
    return shader->compileSourceFile(filePath.absoluteFilePath());
}

QString FileController::log()
{
    return shader->log();
}

QGLShader *FileController::getShader(void)
{
    return shader;
}

bool FileController::save(const QString& content)
{
    if(!isNew)
    {
        QFile file(this->filePath.absoluteFilePath());
        if(file.open(QIODevice::WriteOnly | QIODevice::Text))
        {
            QTextStream out(&file);
            out << content;
            file.close();
            changed = false;

            return true;
        }
        else
        {
            return false;
        }
    }
    else
    {
        qDebug() << "New file won't be saved for now.";
        return false;
    }

}

QString FileController::getFileName()
{
    if(isNew)
    {
        return QString("new_") + ShaderLab::shaderToStr(shaderType);
    }
    else
    {
        return filePath.fileName();
    }
}

void FileController::setChanged(bool val)
{
    changed = val;
}

bool FileController::getChanged(void)
{
    return changed;
}
