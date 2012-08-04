#include "SLShader2.h"
#include "EditorController.h"
#include "SLFile.h"
#include "SLShaderProgram.h"

//constructors
SLShader2::SLShader2(const QString &filePath, ShaderLab::Shader shadertype, SLShaderProgram *parent):
    QObject(parent), m_type(shadertype), m_compiled(false),
    m_log(QString()), m_activated(true),
    m_editor(new EditorController(shadertype, filePath, parent)), m_shaderId(0)
{
    m_shaderId = glCreateShader(glShaderType());

    connect(m_editor, SIGNAL(useless(ShaderLab::Shader)),
            parent, SLOT(uselessEditor(ShaderLab::Shader)));
}

//destructors
SLShader2::~SLShader2()
{
    m_editor->closeShaderCode();
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
        return GL_GEOMETRY_SHADER;
    case ShaderLab::TessellationCtrl:
        return GL_TESS_CONTROL_SHADER;
    case ShaderLab::TessellationEval:
        return GL_TESS_EVALUATION_SHADER;
    default:
        return -1;
    }
}

//public methods
bool SLShader2::compile()
{
    m_compiled = false;

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
    {
        return m_editor->getFile().getFilePath();
    }
    return QString();
}

EditorController* SLShader2::editor()
{
    return m_editor;
}

void SLShader2::setActivated(bool value)
{
    m_activated = value;
}

bool SLShader2::isActivated()
{
    return m_activated;
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
