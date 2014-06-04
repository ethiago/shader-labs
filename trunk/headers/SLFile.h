#ifndef SLFILE_H
#define SLFILE_H

#include <QObject>
#include <QFileInfo>

#include "Global.h"

class SLFile : public QObject
{

public:
    static bool isValid(QString filepath);
    static QString getFileName(const QString& filepath);
    static QString fileNameWithoutExt(const QString& filepath);

    explicit SLFile(ShaderLab::Shader shadertype,const QString& filepath = QString(), QObject *parent = 0);
    ~SLFile();

    QString getFileContent(void) const;
    QString getFileName() const;
    QString getFilePath(void) const;

    bool getChanged(void) const;
    bool isNew(void) const;
    ShaderLab::Shader shaderType(void) const;

    void setChanged(bool val);
    bool isSaved();
    void setFilePath(const QString &filepath);

    bool save(const QString& content);

    bool isActive(void) const;
    void setActive(bool);

private:
    QFileInfo filePath; /* Path to the file which the current instance is responsible for. */

    bool changed;       /* Indicates that the content of a file has been changed. */
    bool m_new;         /* Indicates that the current instance has been created for a new file. */
    bool active;

    ShaderLab::Shader m_shaderType; /* The kind of shader that the instance will manage. */

};

#endif // SLFILE_H
