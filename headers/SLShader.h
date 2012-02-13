#ifndef SLSHADER_H
#define SLSHADER_H

#include <QObject>
#include <QGLShaderProgram>
#include <Global.h>
#include "SLFile.h"

class SLShader : public QObject
{
    Q_OBJECT

    SLFile vertexShader;
    SLFile geometryShader;
    SLFile fragmentShader;

    QGLShaderProgram program;

    QString m_log;

    void logProcess(const SLFile&);
public:
    explicit SLShader(QObject *parent = 0);

    void compileAndLink(GLenum inputType, GLenum outputType);
    const QString& log();
    void bind();
    void release();

    void setShader(const QString& filePath, ShaderLab::Shader shadertype);

signals:

public slots:

};

#endif // SLSHADER_H
