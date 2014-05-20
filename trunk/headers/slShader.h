#ifndef SLSHADER_H
#define SLSHADER_H

#include <QObject>

#include "Global.h"
#include "gl3w.h"

class SLShader : public QObject
{
    Q_OBJECT

    ShaderLab::Shader m_type;
    bool m_compiled;
    QString m_log;
    GLint m_shaderId;

    void sendCodeToOpenGL(const QString&);

public:
    explicit SLShader(ShaderLab::Shader);
    ~SLShader();

    const bool& isCompiled(void)const;
    const QString& log()const;
    const ShaderLab::Shader& type()const;
    const GLint& shaderId()const;

    bool compile(const QString&);
};

#endif // SLSHADER_H
