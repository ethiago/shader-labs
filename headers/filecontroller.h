#ifndef FILECONTROLLER_H
#define FILECONTROLLER_H

#include <QObject>
#include <QGLShader>
#include <QFileInfo>

#include "global.h"

class FileController : public QObject
{
    Q_OBJECT

public:
    explicit FileController(QString filepath, ShaderLab::Shader shadertype, QObject *parent = 0);
    explicit FileController(ShaderLab::Shader shadertype, QObject *parent = 0);
    ~FileController();

    QString getFileContent(void);
    QString getFileName();
    QGLShader* getShader(void);
    bool getChanged(void);
    void setChanged(bool val);

    bool compile(const QString& code);
    bool compile(void);
    QString log();
    bool save(const QString&);

private: 
    QFileInfo filePath; /* Path to the file which the current instance is responsible for. */
    QGLShader *shader;  /* QT resource to compile a shader program. */

    bool changed;       /* Indicates that the content of a file has been changed. */
    bool isNew;         /* Indicates that the current instance has been crated for a new file. */

    /* The kind of shader that the instance will manage. */
    ShaderLab::Shader shaderType;


};

#endif // FILECONTROLLER_H
