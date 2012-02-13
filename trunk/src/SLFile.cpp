#include <QString>
#include <QFile>

#include "SLFile.h"

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* +++++++++++++++++ Constructors and destructors ++++++++++++++++++ */

/* Constructor for a file that already exists */
SLFile::SLFile(QString filepath, ShaderLab::Shader shadertype, QObject *parent) : QObject(parent)
{
    filePath = filepath;
    m_shaderType = shadertype;
    isNew = false;
    changed = false;
    active = true;

    shader = new QGLShader(ShaderLab::shaderToQGLShader(m_shaderType), this);

}

/* Constructor for a new file, created inside ShaderLab */
SLFile::SLFile(ShaderLab::Shader shadertype, QObject *parent) : QObject(parent)
{
    m_shaderType = shadertype;
    isNew = true;
    changed = true;
    active = true;

    shader = new QGLShader(ShaderLab::shaderToQGLShader(m_shaderType));
}

/* Class destructor */
SLFile::~SLFile()
{
    delete shader;
}


/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* ++++++++++++++++++++++ Getters and setters ++++++++++++++++++++++ */

/* Returns the content of the file IN THE DISK. */
QString SLFile::getFileContent(void) const
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
QString SLFile::getFileName() const
{
    if(isNew)
    {
        return QString("new_") + ShaderLab::shaderToStr(m_shaderType);
    }
    else
    {
        return filePath.fileName();
    }
}

/* Getter for the QGLShader attribute. */
QGLShader *SLFile::getShader(void) const
{
    return shader;
}

/* Getter for the changed attribute. */
bool SLFile::getChanged(void) const
{
    return changed;
}

/* Getter for the shaderLabType attribute. */
ShaderLab::Shader SLFile::shaderType(void) const
{
    return m_shaderType;
}

/* Getter for the active attribute. */
bool SLFile::isActive(void) const
{
    return active;
}

/* Getter for the isNew attribute. */
bool SLFile::IsNew(void) const
{
    return isNew;
}

/* Setter for the changed attribute. */
void SLFile::setChanged(bool val)
{
    changed = val;
}

/* Setter for the filePath attribute. */
void SLFile::setFilePath(const QString &filepath)
{
    filePath = QFileInfo(filepath);
}

/* Setter for the active attribute. */
void SLFile::setActive(bool a)
{
    active = a;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* +++++++++++++++++++++++++ Other methods +++++++++++++++++++++++++ */

/* Compiles the source code from a file in the disk, given a path to it. */
bool SLFile::compile(void)
{
    return shader->compileSourceFile(filePath.absoluteFilePath());
}

/* Compiles the code currently in the screen, given the code. */
bool SLFile::compile(const QString& code)
{

    delete shader;
    shader = new QGLShader(ShaderLab::shaderToQGLShader(m_shaderType), this);
    return shader->compileSourceCode(code);

}

/* Returns the log from the last compilation process. */
QString SLFile::log() const
{
    return shader->log();
}

/* Saves the given content on the file. */
/* Assumes that filepath has been set for all cases (new or old files) */
bool SLFile::save(const QString& content)
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

bool SLFile::isValid(QString filepath)
{
    QFile file(filepath);
    bool ret;
    if(file.open(QFile::ReadOnly))
        ret = true;
    else
        ret = false;

    return ret;
}

QString SLFile::getFilePath(void) const
{
    return filePath.absoluteFilePath();
}

