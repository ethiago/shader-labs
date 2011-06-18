#include <QString>
#include <QFile>

#include "filecontroller.h"

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

/* Returns the name of the associated file. New files are given a default name. */
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

/* Getter for the QGLShader attribute. */
QGLShader *FileController::getShader(void)
{
    return shader;
}

/* Getter for the changed attribute. */
bool FileController::getChanged(void)
{
    return changed;
}

/* Getter for the shaderLabType attribute. */
ShaderLab::Shader FileController::getShaderType(void)
{
    return shaderType;
}

/* Getter for the isNew attribute. */
bool FileController::IsNew(void)
{
    return isNew;
}

/* Setter for the changed attribute. */
void FileController::setChanged(bool val)
{
    changed = val;
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
QString FileController::log()
{
    return shader->log();
}

bool FileController::isActive(void)
{
    return active;
}

void FileController::setActive(bool a)
{
    active = a;
}






/***************************************************************
****************************************************************

                     REDUNDANTEEEEEEEEE!!!!

***************************************************************
****************************************************************/



/* Saves the given content on the file. */
/* TO-DO: Save a new file */
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
        else return false;
    }
    else
    {
        qDebug() << "New file won't be saved for now.";
        return false;
    }
}

bool FileController::saveAsNewFile(const QString& filepath, const QString& fileContent)
{
    filePath = filepath;

    QFile file(this->filePath.absoluteFilePath());
    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << fileContent;
        file.close();
        changed = false;
        isNew = false;

        return true;
    }
    else
    {
        return false;
    }
}
