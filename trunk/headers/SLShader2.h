#ifndef SLSHADER2_H
#define SLSHADER2_H

#define GL3_PROTOTYPES
#include <GL/gl3.h>
#define GL_GLEXT_PROTOTYPES
#include <GL/glext.h>
#include <QObject>
#include "Global.h"

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
