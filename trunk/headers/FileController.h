#ifndef FILECONTROLLER_H
#define FILECONTROLLER_H

#include <QObject>
#include <QGLShader>
#include <QFileInfo>

#include "Global.h"

class FileController : public QObject
{
    Q_OBJECT

public:
    static bool isValid(QString filepath);

    explicit FileController(QString filepath, ShaderLab::Shader shadertype, QObject *parent = 0);
    explicit FileController(ShaderLab::Shader shadertype, QObject *parent = 0);
    ~FileController();

    QString getFileContent(void) const;
    QString getFileName() const;
    QString getFilePath(void) const;

    QGLShader* getShader(void) const;
    bool getChanged(void) const;
    bool IsNew(void) const;
    ShaderLab::Shader getShaderType(void) const;

    void setChanged(bool val);
    void setFilePath(const QString &filepath);

    bool compile(const QString& code);
    bool compile(void);
    QString log() const;
    bool save(const QString& content);

    bool isActive(void) const;
    void setActive(bool);

private:
    QFileInfo filePath; /* Path to the file which the current instance is responsible for. */
    QGLShader *shader;  /* QT resource to compile a shader program. */

    bool changed;       /* Indicates that the content of a file has been changed. */
    bool isNew;         /* Indicates that the current instance has been crated for a new file. */
    bool active;

    ShaderLab::Shader shaderType; /* The kind of shader that the instance will manage. */

};

#endif // FILECONTROLLER_H
