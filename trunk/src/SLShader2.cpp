#include "SLShader2.h"
#include "EditorController.h"
#include "SLFile.h"
#include "SLShaderProgram.h"

#include <QVarLengthArray>
#include <QDebug>

#ifndef GL_GEOMETRY_SHADER_EXT
#define GL_GEOMETRY_SHADER_EXT                      0x8DD9
#endif

//constructors
SLShader2::SLShader2(const QString &filePath, ShaderLab::Shader shadertype, SLShaderProgram *parent):
    QObject(parent), m_type(shadertype), m_compiled(false),
    m_log(QString()),  m_editor(new EditorController(shadertype, filePath, parent)),
    m_shaderId(0)
{

    m_shaderId = glCreateShader(glShaderType());

    connect(m_editor, SIGNAL(useless(ShaderLab::Shader)),
            parent, SLOT(uselessEditor(ShaderLab::Shader)));
}

//destructors
SLShader2::~SLShader2()
{
    delete m_editor;
    glDeleteShader(m_shaderId);
}

//Privates methods
GLenum SLShader2::glShaderType()
{
    switch(m_type)
    {
    case ShaderLab::Vertex:
        return GL_VERTEX_SHADER;
    case ShaderLab::Fragment:
        return GL_FRAGMENT_SHADER;
    case ShaderLab::Geometry:
        if(ShaderLab::instance()->geometryShaderOnlyEXT())
            return GL_GEOMETRY_SHADER_EXT;
        else
            return GL_GEOMETRY_SHADER;
    case ShaderLab::TessellationCtrl:
        return GL_TESS_CONTROL_SHADER;
    case ShaderLab::TessellationEval:
        return GL_TESS_EVALUATION_SHADER;
    default:
        return -1;
    }
}

void printArr(const char ** strings, int * len, int count)
{
    qDebug() << "#######################################";
    for(int i = 0; i < count; ++i)
    {
        QByteArray a(strings[i], len[i]);
        qDebug() << QString(a);
    }
    qDebug() << "#######################################";
}

void SLShader2::bindShaderCode()
{
    QByteArray srcArr = m_editor->getContent().toLatin1();
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

    //printArr(src.data(), srclen.data(), src.size());
    glShaderSource(m_shaderId, src.size(), src.data(), srclen.data());
}

bool SLShader2::priv_compile()
{
    m_compiled = false;
    m_log = "";

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
        m_log = QString::fromLatin1(logbuf);
        QString name = ShaderLab::instance()->shaderToStrCap(m_type);
        if (name.isEmpty())
            qWarning() << "SLShader::compile:" << m_log;
        else
            qWarning() << "SLShader::compile[" << name << "]:" << m_log;
        delete [] logbuf;
    }

    return m_compiled;
}

//public methods
bool SLShader2::compile()
{
    bindShaderCode();
    return priv_compile();
}

const QString& SLShader2::log()
{
    return m_log;
}

bool SLShader2::closeFile()
{
    if(!m_editor->closeShaderCode())
        return false;

    delete m_editor;
    m_editor = NULL;
    return true;
}

bool SLShader2::save()
{
    return m_editor->saveFile();
}

QString SLShader2::getAbsoluteFilePath()
{
    if (!m_editor->getFile().isNew())
        return m_editor->getFile().getFilePath();

    return QString();
}

EditorController* SLShader2::editor()
{
    return m_editor;
}

bool SLShader2::isActivated()
{
    return m_editor->isActive();
}

ShaderLab::Shader SLShader2::type()
{
    return m_type;
}

GLint SLShader2::shaderId()
{
    return m_shaderId;
}

bool SLShader2::isNew()
{
    return editor()->isNew();
}
