#ifndef SLSHADERPROGRAM_H
#define SLSHADERPROGRAM_H

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
#include <QObject>
#include <Global.h>

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

    bool geometryAttached;
    bool m_linked;


    int getShaderIndex(ShaderLab::Shader);
    void removeAllShaders();
    QString logProcess(SLShader2* shader);
    bool programLink();
public:
    explicit SLShaderProgram(QObject *parent = 0);
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
    void setUniformValue(const char *name, const QVector2D& value);
    void setUniformValue(const char *name, const QSize& value);
    void setUniformValue(const char *name, GLuint value);
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
