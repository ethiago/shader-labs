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
    explicit FileController(QString filepath, ShaderLab::Shader shadertype,
                            QObject *parent = 0);

    explicit FileController(ShaderLab::Shader shadertype, QObject *parent = 0);

    ~FileController();

    QString getFileContent(void);

    bool compile(const QString& code);
    bool compile(void);
    QGLShader* getShader(void);
    QString log();
    bool save(const QString&);
    QString getFileName();
    void setChanged(bool val);
    bool getChanged(void);


private:
    QFileInfo filePath;
    QGLShader *shader;
    ShaderLab::Shader shaderType;
    bool isNew;
    bool changed;

};

#endif // FILECONTROLLER_H
