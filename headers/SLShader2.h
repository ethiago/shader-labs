#ifndef SLSHADER2_H
#define SLSHADER2_H

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
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
    bool m_activated;

    EditorController* m_editor;
    GLint m_shaderId;

    GLenum glShaderType();

public:
    explicit SLShader2(const QString& filePath, ShaderLab::Shader shadertype, SLShaderProgram *parent = 0);
    ~SLShader2();

    bool compile();
    const QString& log();
    bool closeFile();
    bool save();
    QString getAbsoluteFilePath();
    EditorController* editor();
    void setActivated(bool);
    bool isActivated();
    ShaderLab::Shader type();
    GLint shaderId();
    bool isNew();

};

#endif // SLSHADER2_H
