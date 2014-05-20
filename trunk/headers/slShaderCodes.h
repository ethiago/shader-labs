#ifndef SLSHADERCODES_H
#define SLSHADERCODES_H

#include <QObject>
#include <QList>

#include "Global.h"
#include "gl3w.h"

class SLShader;

class SLShaderCodes : public QObject
{
    Q_OBJECT

    QList<bool> m_active;
    QList<QString> m_lastSource;
    QList<QString> m_filePath;
    QList<SLShader*> m_shaders;

    QList<GLuint> m_successfulCompiles;

    QString m_log;

    bool m_hasCompiledGeometry;

public:
    explicit SLShaderCodes();

    const QList<GLuint>& listForLink()const;

    void setActive(ShaderLab::Shader, bool);
    const QString& lastSource(ShaderLab::Shader)const;
    bool isActive(ShaderLab::Shader)const;
    void closeShader(ShaderLab::Shader);
    bool save(ShaderLab::Shader);
    bool saveAs(ShaderLab::Shader, const QString& filePath);
    bool openShader(ShaderLab::Shader, const QString& filePath);
    void newShader(ShaderLab::Shader);
    void updateCode(ShaderLab::Shader shaderType, const QString& code);
    bool isNew(ShaderLab::Shader shaderType)const;
    QString fileName(ShaderLab::Shader)const;
    const QString& filePath(ShaderLab::Shader)const;

    void clearShaders();

    void compile();

    bool hasCompiledGeometry()const;

    const QString& log()const;

    bool isChanged(ShaderLab::Shader shaderType);
    
};

#endif // SLSHADERCODES_H
