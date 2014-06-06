#include "slShaderProgram2.h"
#include "slShaderCodes.h"
#include "slgl3w.h"

#include <QDebug>
#include <QSize>
#include <QGLWidget>

#ifndef GL_GEOMETRY_VERTICES_OUT_EXT
#define GL_GEOMETRY_VERTICES_OUT_EXT                0x8DDA
#endif

#ifndef GL_GEOMETRY_INPUT_TYPE_EXT
#define GL_GEOMETRY_INPUT_TYPE_EXT                  0x8DDB
#endif

#ifndef GL_GEOMETRY_OUTPUT_TYPE_EXT
#define GL_GEOMETRY_OUTPUT_TYPE_EXT                    0x8DDC
#endif

#ifndef GL_LINES_ADJACENCY_EXT
#define GL_LINES_ADJACENCY_EXT                         0xA
#endif

#ifndef GL_LINE_STRIP_ADJACENCY_EXT
#define GL_LINE_STRIP_ADJACENCY_EXT                    0xB
#endif

#ifndef GL_TRIANGLES_ADJACENCY_EXT
#define GL_TRIANGLES_ADJACENCY_EXT                     0xC
#endif

#ifndef GL_TRIANGLE_STRIP_ADJACENCY_EXT
#define GL_TRIANGLE_STRIP_ADJACENCY_EXT                0xD
#endif

SLShaderProgram2::SLShaderProgram2() :
    QObject(NULL)
{
    m_programId = glCreateProgram();
}

SLShaderProgram2::~SLShaderProgram2()
{
    glDeleteProgram(m_programId);
}

void SLShaderProgram2::bind() const
{
    if(m_linked)
    {
        glUseProgram(m_programId);

        ShaderLab *sl = ShaderLab::instance();
        if(m_sizeLocation >= 0)
        {
            QSize value = sl->glContext()->size();
            glUniform2f(m_sizeLocation, value.width(), value.height());
        }

        if(m_timeLocation >= 0)
        {
            glUniform1f(m_timeLocation, sl->time());
        }
    }
}

void SLShaderProgram2::release()const
{
    glUseProgram(0);
}

bool SLShaderProgram2::isLinked()const
{
    return m_linked;
}

const QString& SLShaderProgram2::log()const
{
    return m_log;
}

GLuint SLShaderProgram2::programId()const
{
    return m_programId;
}



bool SLShaderProgram2::link(const QList<GLuint>& shaders)
{

    SLShaderProgramAlgorithm alg(m_programId);
    release();
    alg.detachShaders();
    m_linked = false;
    m_log = "";

    m_log += "====================== Linking process ======================\n";

    if(shaders.size() == 0)
    {
        m_log += tr("No compiled shaders to link.\n");
        return m_linked;
    }

    alg.attachShaders(shaders);

    m_log += alg.link(&m_linked);

    if(isLinked())
    {
        m_sizeLocation = SLGl3W::getUniformLocation(m_programId, "wsize");
        m_timeLocation = SLGl3W::getUniformLocation(m_programId,  "time");
    }

    return m_linked;
}

void SLShaderProgram2::setGeometrySettings(GLint inputType, GLint outputType, GLint vertexCount)const
{
    glProgramParameteriEXT(m_programId, GL_GEOMETRY_INPUT_TYPE_EXT,
                                   inputType);
    glProgramParameteriEXT(m_programId, GL_GEOMETRY_OUTPUT_TYPE_EXT,
                                   outputType);
    glProgramParameteriEXT(m_programId, GL_GEOMETRY_VERTICES_OUT_EXT,
                                   vertexCount);
}

SLShaderProgramAlgorithm::SLShaderProgramAlgorithm(GLuint programId): e_programId(programId)
{
}

void SLShaderProgramAlgorithm::detachShaders()const
{
    GLsizei max = 5;
    GLuint shaders[max];
    GLsizei count = 0;

    glGetAttachedShaders(e_programId, max, &count, shaders);

    for(int i = 0; i < count; ++i)
        glDetachShader(e_programId, shaders[i]);
}

void SLShaderProgramAlgorithm::attachShaders(const QList<GLuint>& shadersId)const
{
    for(int i = 0; i < shadersId.size(); ++i)
    {
        glAttachShader(e_programId, shadersId[i]);
    }
}

QString SLShaderProgramAlgorithm::link(bool *isLinked) const
{
    QString log;
    glLinkProgram(e_programId);
    int value = 0;
    glGetProgramiv(e_programId, GL_LINK_STATUS, &value);
    *isLinked = (value != 0);
    value = 0;
    glGetProgramiv(e_programId, GL_INFO_LOG_LENGTH, &value);
    log = QString();
    if (value > 1) {
        char *logbuf = new char [value];
        GLint len;
        glGetProgramInfoLog(e_programId, value, &len, logbuf);
        log = QString::fromLatin1(logbuf);
        qWarning() << "SLShaderProgram::link:" << log;
        delete [] logbuf;
    }else
        log = "Successful.";
    log+="\n";

    return log;
}

