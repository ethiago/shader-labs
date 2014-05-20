#ifndef SLSHADERPROGRAM2_H
#define SLSHADERPROGRAM2_H

#include <QObject>

#include "Global.h"
#include "gl3w.h"

class SLShaderCodes;

class SLShaderProgram2 : public QObject
{
    Q_OBJECT

    GLuint m_programId;

    bool m_linked;

    QString m_log;

    GLint m_sizeLocation;
    GLint m_timeLocation;


public:
    explicit SLShaderProgram2();
    ~SLShaderProgram2();

    void bind()const;
    void release()const;
    bool isLinked()const;
    GLuint programId()const;
    bool link(const QList<GLuint> &shaders);

    const QString& log()const;

    static GLint getUniformLocation(GLuint program, const QString& name);

    void setGeometrySettings(GLint inputType, GLint outputType, GLint vertexCount)const;

};


class SLShaderProgramAlgorithm
{
    GLuint e_programId;

public:
    SLShaderProgramAlgorithm(GLuint programId);
    void detachShaders() const;
    void attachShaders(const QList<GLuint> &shadersId)const;
    QString link(bool*) const;

};

#endif // SLSHADERPROGRAM2_H
