#include <QString>
#include <QFile>

#include "FileController.h"

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* +++++++++++++++++ Constructors and destructors ++++++++++++++++++ */

/* Constructor for a file that already exists */
FileController::FileController(QString filepath, ShaderLab::Shader shadertype, QObject *parent) : QObject(parent)
{
    filePath = filepath;
    shaderType = shadertype;
    isNew = false;
    changed = false;
    active = true;

    shader = new QGLShader(ShaderLab::shaderToQGLShader(shaderType), this);

}

/* Constructor for a new file, created inside ShaderLab */
FileController::FileController(ShaderLab::Shader shadertype, QObject *parent) : QObject(parent)
{
    shaderType = shadertype;
    isNew = true;
    changed = true;
    active = true;

    shader = new QGLShader(ShaderLab::shaderToQGLShader(shadertype));
}

/* Class destructor */
FileController::~FileController()
{
    delete shader;
}


/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* ++++++++++++++++++++++ Getters and setters ++++++++++++++++++++++ */

/* Returns the content of the file IN THE DISK. */
QString FileController::getFileContent(void) const
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

/* Returns the name of the associated file. New files are given a default name. */
QString FileController::getFileName() const
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

/* Getter for the QGLShader attribute. */
QGLShader *FileController::getShader(void) const
{
    return shader;
}

/* Getter for the changed attribute. */
bool FileController::getChanged(void) const
{
    return changed;
}

/* Getter for the shaderLabType attribute. */
ShaderLab::Shader FileController::getShaderType(void) const
{
    return shaderType;
}

/* Getter for the active attribute. */
bool FileController::isActive(void) const
{
    return active;
}

/* Getter for the isNew attribute. */
bool FileController::IsNew(void) const
{
    return isNew;
}

/* Setter for the changed attribute. */
void FileController::setChanged(bool val)
{
    changed = val;
}

/* Setter for the filePath attribute. */
void FileController::setFilePath(const QString &filepath)
{
    filePath = QFileInfo(filepath);
}

/* Setter for the active attribute. */
void FileController::setActive(bool a)
{
    active = a;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* +++++++++++++++++++++++++ Other methods +++++++++++++++++++++++++ */

/* Compiles the source code from a file in the disk, given a path to it. */
bool FileController::compile(void)
{
    return shader->compileSourceFile(filePath.absoluteFilePath());
}

/* Compiles the code currently in the screen, given the code. */
bool FileController::compile(const QString& code)
{

    delete shader;
    shader = new QGLShader(ShaderLab::shaderToQGLShader(shaderType), this);
    return shader->compileSourceCode(code);

}

/* Returns the log from the last compilation process. */
QString FileController::log() const
{
    return shader->log();
}

/* Saves the given content on the file. */
/* Assumes that filepath has been set for all cases (new or old files) */
bool FileController::save(const QString& content)
{
    QFile file(this->filePath.absoluteFilePath());

    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << content;
        file.close();
        changed = false;
        isNew = false;

        return true;
    }
    else return false;
}

bool FileController::isValid(QString filepath)
{
    QFile file(filepath);
    bool ret;
    if(file.open(QFile::ReadOnly))
        ret = true;
    else
        ret = false;

    return ret;
}

QString FileController::getFilePath(void) const
{
    return filePath.absoluteFilePath();
}

