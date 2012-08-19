#ifndef SLSHADER2_H
#define SLSHADER2_H

#include <QObject>

#include "Global.h"
#include "gl3w.h"

class EditorController;
class SLShaderProgram;

class SLShader2 : public QObject
{
    Q_OBJECT

    ShaderLab::Shader m_type;
    bool m_compiled;
    QString m_log;

    EditorController* m_editor;
    GLint m_shaderId;

    GLenum glShaderType();

    void bindShaderCode();
    bool priv_compile();

public:
    explicit SLShader2(const QString& filePath, ShaderLab::Shader shadertype, SLShaderProgram *parent = 0);
    ~SLShader2();


    const QString& log();
    bool closeFile();
    bool save();
    QString getAbsoluteFilePath();
    EditorController* editor();
    bool isActivated();
    ShaderLab::Shader type();
    GLint shaderId();
    bool isNew();
    bool compile();

};

#endif // SLSHADER2_H
