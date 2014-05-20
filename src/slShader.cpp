#include "slShader.h"

#include <QVarLengthArray>
#include <QDebug>

SLShader::SLShader(ShaderLab::Shader shadertype) :
    QObject(NULL), m_type(shadertype), m_compiled(false), m_log(QString())
{
    ShaderLab * sl = ShaderLab::instance();
    m_shaderId = glCreateShader(sl->shaderToGLShader(m_type));
}

SLShader::~SLShader()
{
    glDeleteShader(m_shaderId);
}

const bool& SLShader::isCompiled(void)const
{
    return m_compiled;
}

const QString& SLShader::log()const
{
    return m_log;
}

const ShaderLab::Shader& SLShader::type()const
{
    return m_type;
}

const GLint& SLShader::shaderId()const
{
    return m_shaderId;
}

bool SLShader::compile(const QString & code)
{
    sendCodeToOpenGL(code);

    m_log += "==================== Compiling "+ ShaderLab::shaderToStr(m_type) +" code ====================\n";

    m_compiled = false;

    QString log;
    glCompileShader(m_shaderId);
    GLint value = 0;
    glGetShaderiv(m_shaderId, GL_COMPILE_STATUS, &value);
    m_compiled = (value != 0);
    value = 0;
    glGetShaderiv(m_shaderId, GL_INFO_LOG_LENGTH, &value);
    if (!m_compiled && value > 1) {
        char *logbuf = new char [value];
        GLint len;
        glGetShaderInfoLog(m_shaderId, value, &len, logbuf);
        log = QString::fromLatin1(logbuf);
        m_log += log;
        qWarning() << "SLShader::compile[" << ShaderLab::shaderToStr(m_type) << "]:" << log;
        delete [] logbuf;
    }

    if(log.isEmpty())
    {
        m_log += "Successfull.";
    }
    m_log += "\n";

    return m_compiled;
}


//PVT
void SLShader::sendCodeToOpenGL(const QString& code)
{
    QByteArray srcArr = code.toLatin1();
    const char * source = srcArr.constData();

    QVarLengthArray<const char *, 4> src;
    QVarLengthArray<GLint, 4> srclen;
    int headerLen = 0;

    while (source && source[headerLen] == '#') {
        // Skip #version and #extension directives at the start of
        // the shader code.  We need to insert the qualifierDefines
        // and redefineHighp just after them.
        if (qstrncmp(source + headerLen, "#version", 8) != 0 &&
                qstrncmp(source + headerLen, "#extension", 10) != 0) {
            break;
        }
        while (source[headerLen] != '\0' && source[headerLen] != '\n')
            ++headerLen;
        if (source[headerLen] == '\n')
            ++headerLen;
    }
    if (headerLen > 0) {
        src.append(source);
        srclen.append(GLint(headerLen));
    }

    src.append(source + headerLen);
    srclen.append(GLint(qstrlen(source + headerLen)));

    glShaderSource(m_shaderId, src.size(), src.data(), srclen.data());
}
