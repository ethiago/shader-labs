#include "SLFile.h"

#include <QString>
#include <QFile>
#include <QTextStream>

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* +++++++++++++++++ Constructors and destructors ++++++++++++++++++ */

/* Constructor for a file that already exists */
SLFile::SLFile(ShaderLab::Shader shadertype,const QString& filepath, QObject *parent) : QObject(parent)
{
     m_shaderType = shadertype;
     active = true;
     if(SLFile::isValid(filepath))
    {
        filePath = filepath;
        m_new = false;
        changed = false;
    }else
    {
        m_new = true;
        changed = true;
    }

}

/* Class destructor */
SLFile::~SLFile()
{
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
    if(m_new)
    {
        return QString("new_") + ShaderLab::shaderToStr(m_shaderType);
    }
    else
    {
        return filePath.fileName();
    }
}

QString SLFile::getFileName(const QString& filepath)
{

    QFileInfo fp(filepath);
    return fp.fileName();
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
bool SLFile::isNew(void) const
{
    return m_new;
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
        m_new = false;

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

bool SLFile::isSaved()
{
    return !changed;
}
