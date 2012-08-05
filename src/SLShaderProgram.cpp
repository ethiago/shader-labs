#include "SLShaderProgram.h"
#include "SLShader2.h"

//constructors
SLShaderProgram::SLShaderProgram(QObject *parent) :
    QObject(parent), m_programId(0), m_log(QString()),
    m_geometryInputType(GL_TRIANGLES), m_geometryOutputType(GL_TRIANGLE_STRIP),
    m_geometryVertexCount(64), m_linked(false)
{
    m_programId = glCreateProgram();
    geometryAttached = false;
}

//destructor
SLShaderProgram::~SLShaderProgram()
{
    glDeleteProgram(m_programId);

    for(int i = 0; i < m_shaders.size(); ++i)
    {
        delete m_shaders[i];
    }
}

//private methods
int SLShaderProgram::getShaderIndex(ShaderLab::Shader type)
{
    for(int i = 0; i < m_shaders.size(); ++i)
    {
        if(m_shaders[i]->type() == type)
            return i;
    }
    return -1;
}

void SLShaderProgram::removeAllShaders()
{
    for(int i = 0; i < m_shaders.size(); ++i)
    {
        glDetachShader(m_programId, m_shaders[i]->shaderId());
    }
    geometryAttached = false;
    m_linked = false;
}

void SLShaderProgram::release()
{
    GLuint noProgramId = 0;
    glUseProgram(noProgramId);
}

QString SLShaderProgram::logProcess(SLShader2* shader)
{
    QString log;
    log += "==================== Compiling "+ ShaderLab::shaderToStr(shader->type()) +" code ====================\n";
    QString ShaderLog = shader->log();

    if(ShaderLog == "")
        log += "Successfull.\n";
    else
        log += ShaderLog + "\n";
    return log;
}

bool SLShaderProgram::programLink()
{
    if(geometryAttached)
    {
        glProgramParameteriARB(m_programId, GL_GEOMETRY_INPUT_TYPE_EXT,
                               m_geometryInputType);
        glProgramParameteriARB(m_programId, GL_GEOMETRY_OUTPUT_TYPE_EXT,
                               m_geometryOutputType);
        glProgramParameteriARB(m_programId, GL_GEOMETRY_VERTICES_OUT_EXT,
                               m_geometryVertexCount);
    }

    glLinkProgram(m_programId);
    int value = 0;
    glGetProgramiv(m_programId, GL_LINK_STATUS, &value);
    m_linked = (value != 0);
    value = 0;
    glGetProgramiv(m_programId, GL_INFO_LOG_LENGTH, &value);
    m_log = QString();
    if (value > 1) {
        char *logbuf = new char [value];
        GLint len;
        glGetProgramInfoLog(m_programId, value, &len, logbuf);
        m_log = QString::fromLatin1(logbuf);
        QString name = "SLShaderProgram";
        if (name.isEmpty())
            qWarning() << "SLShader::link:" << m_log;
        else
            qWarning() << "SLShader::link[" << name << "]:" << m_log;
        delete [] logbuf;
    }
    return m_linked;
}

//public methods
EditorController* SLShaderProgram::setShader(ShaderLab::Shader type, const QString& filePath)
{
    SLShader2 * shader;
    int ind = getShaderIndex(type);

    if(ind != -1)
    {
        shader = m_shaders[ind];
        if(!shader->closeFile())
            return NULL;

        m_shaders.removeAt(ind);
        delete shader;
    }

    shader = new SLShader2(filePath,type,this);
    m_shaders.append(shader);

    return shader->editor();
}

bool SLShaderProgram::compileAndLink()
{
    removeAllShaders();
    release();

    bool atLeastOne = false;
    bool thereIsCode = false;

    QString log = "";

    for(int i = 0; i < m_shaders.size(); ++i)
    {
        if(m_shaders[i]->isActivated())
        {
            thereIsCode = true;
            if(m_shaders[i]->compile())
            {
                glAttachShader(m_programId, m_shaders[i]->shaderId());
                atLeastOne = true;
                if(m_shaders[i]->type() == ShaderLab::Geometry)
                    geometryAttached = true;
            }
            log += logProcess(m_shaders[i]);
        }
    }
    log += "====================== Linking process ======================\n";
    bool ret = true;
    if(atLeastOne)
    {
        programLink();
        log += m_log;
    }
    else
    {
        log += "Due to problems, linking process could not be performed.";
        ret = false;
    }

    if(!thereIsCode)
        log = tr("No active shader code to compile.");

    m_completeLog = log;
    return ret;
}

SLShader2* SLShaderProgram::getShader(ShaderLab::Shader type)
{
    return m_shaders[getShaderIndex(type)];
}

void SLShaderProgram::setGeometryInputType(GLenum type)
{
    m_geometryInputType = type;
}

void SLShaderProgram::setGeometryOutputType(GLenum type)
{
    m_geometryOutputType = type;
}

void SLShaderProgram::setGeometryVertexCount(GLuint n)
{
    m_geometryVertexCount = n;
}

bool SLShaderProgram::isLinked()
{
    return m_linked;
}

void SLShaderProgram::bind()
{
    if(m_linked)
        glUseProgram(m_programId);
    else
        glUseProgram(0);
}

void SLShaderProgram::setUniformValue(const char *name, const QVector2D& value)
{
    GLuint location = 0;

    if (m_linked)
        location = glGetUniformLocation(m_programId, name);

    if (location != 0)
        glUniform2fv(location, 1, reinterpret_cast<const GLfloat *>(&value));
}

void SLShaderProgram::setUniformValue(const char *name, const QSize& value)
{
    GLuint location = 0;

    if (m_linked)
        location = glGetUniformLocation(m_programId, name);

    if (location != 0)
    {
        GLfloat values[4] = {GLfloat(value.width()), GLfloat(value.height())};
        glUniform2fv(location, 1, values);
    }
}



void SLShaderProgram::setUniformValue(const char *name, GLuint value)
{
    GLuint location = 0;

    if (m_linked)
        location = glGetUniformLocation(m_programId, name);

    if (location != 0)
        glUniform1f(location, value);
}

bool SLShaderProgram::saveAllShaders()
{
    bool all = true;

    for(int i = 0; i < m_shaders.size(); ++i)
    {
        if(!m_shaders[i]->save())
            all = false;
    }

    return all;
}

bool SLShaderProgram::isAnyNew()
{
    for(int i = 0; i < m_shaders.size(); ++i)
    {
        if(m_shaders[i]->isNew())
            return true;
    }
    return false;
}

QString SLShaderProgram::getAbsoluteFilePath(ShaderLab::Shader type)
{
    for(int i = 0; i < m_shaders.size(); ++i)
    {
        if(m_shaders[i]->type() == type)
            return m_shaders[i]->getAbsoluteFilePath();
    }
    return QString();
}

bool SLShaderProgram::closeAllShaders()
{
    for(QList<SLShader2*>::iterator it = m_shaders.begin(); it != m_shaders.end(); ++it)
    {
        SLShader2* shader = *it;
        if(!shader->closeFile())
            return false;

        m_shaders.erase(it);
        delete shader;
    }
    return true;
}

//public slots
void SLShaderProgram::uselessEditor(ShaderLab::Shader type)
{
    int ind = getShaderIndex(type);
    SLShader2 * shader = m_shaders[ind];
    m_shaders.removeAt(ind);
    delete shader;
}

QString SLShaderProgram::log()
{
    return m_completeLog;
}

void SLShaderProgram::setStandBy()
{
    for(int i = 0; i < m_shaders.size(); ++i)
    {
        SLShader2 * shader = m_shaders[i];
        delete shader;
    }
    m_shaders.clear();
}
