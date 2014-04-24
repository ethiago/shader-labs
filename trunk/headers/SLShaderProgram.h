#ifndef SLSHADERPROGRAM_H
#define SLSHADERPROGRAM_H

#include <QObject>

#include "Global.h"
#include "gl3w.h"
#include <QVector2D>

class SLShader2;
class EditorController;

class SLShaderProgram : public QObject
{
    Q_OBJECT

    GLuint m_programId;
    QList<SLShader2*> m_shaders;
    QString m_log;
    GLenum m_geometryInputType;
    GLenum m_geometryOutputType;
    GLint m_geometryVertexCount;
    QString m_completeLog;
    GLfloat time;

    GLint wsizeLocation;
    GLint timeLocation;

    bool geometryAttached;
    bool m_linked;
    QList<GLuint> m_attachedShaders;


    int getShaderIndex(ShaderLab::Shader);
    void removeAllShaders();
    QString logProcess(SLShader2* shader);
    bool programLink();
public:
    explicit SLShaderProgram();
    ~SLShaderProgram();
    EditorController* setShader(ShaderLab::Shader, const QString& fileName = QString());
    bool compileAndLink();
    QString log();
    SLShader2* getShader(ShaderLab::Shader);
    void setGeometryInputType(GLenum type);
    void setGeometryOutputType(GLenum type);
    void setGeometryVertexCount(GLuint n);
    bool isLinked();
    void bind();
    int getProgramId();

    GLint uniformLocation(const char *name);
    void setUniformValue(GLuint location, GLuint value);

    void release();
    bool saveAllShaders();
    bool isAnyNew();
    QString getAbsoluteFilePath(ShaderLab::Shader);
    bool closeAllShaders();
    void setStandBy();

public slots:
    void uselessEditor(ShaderLab::Shader);

};

#endif // SLSHADERPROGRAM_H
