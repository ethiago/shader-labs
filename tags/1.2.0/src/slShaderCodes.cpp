#include "slShaderCodes.h"
#include "slShader.h"

#include <QFile>
#include "SLFile.h"

SLShaderCodes::SLShaderCodes() :
    QObject(NULL)
{
    QList<ShaderLab::Shader> list = ShaderLab::shaderTypeList();

    for(int i = 0; i < list.size(); ++i)
    {
        m_active.append(false);
        m_filePath.append(QString());
        m_lastSource.append(QString());
        m_shaders.append(NULL);
    }

    m_log = QString();
    m_successfulCompiles.clear();
    m_hasCompiledGeometry = false;
}

const QList<GLuint>& SLShaderCodes::listForLink()const
{
    return m_successfulCompiles;
}

void SLShaderCodes::setActive(ShaderLab::Shader shaderType, bool active)
{
    unsigned int idx = ShaderLab::shaderToIndex(shaderType);
    m_active[idx] = active;
}

const QString& SLShaderCodes::lastSource(ShaderLab::Shader shaderType)const
{
    unsigned int idx = ShaderLab::shaderToIndex(shaderType);
    return m_lastSource[idx];
}

bool SLShaderCodes::isActive(ShaderLab::Shader shaderType)const
{
    unsigned int idx = ShaderLab::shaderToIndex(shaderType);
    return m_active[idx];
}

void SLShaderCodes::closeShader(ShaderLab::Shader shaderType)
{
    unsigned int idx = ShaderLab::shaderToIndex(shaderType);
    m_active[idx] = false;
    m_lastSource[idx] = QString();
    m_filePath[idx] = QString();
}

void SLShaderCodes::newShader(ShaderLab::Shader shaderType)
{
    unsigned int idx = ShaderLab::shaderToIndex(shaderType);
    m_active[idx] = true;
    m_lastSource[idx] = QString();
    m_filePath[idx] = QString(".");
}

void SLShaderCodes::updateCode(ShaderLab::Shader shaderType, const QString& code)
{
    unsigned int idx = ShaderLab::shaderToIndex(shaderType);
    m_lastSource[idx] = code;
}

bool SLShaderCodes::save(ShaderLab::Shader shaderType)
{
    unsigned int idx = ShaderLab::shaderToIndex(shaderType);

    QFile f(m_filePath[idx]);
    bool ret = f.open(QIODevice::WriteOnly);
    if(ret)
    {
        f.write(m_lastSource[idx].toAscii());
    }
    f.close();

    return ret;
}

bool SLShaderCodes::saveAs(ShaderLab::Shader shaderType, const QString& filePath)
{
    unsigned int idx = ShaderLab::shaderToIndex(shaderType);

    QFile f(filePath);
    bool ret = f.open(QIODevice::WriteOnly);
    if(ret)
    {
        f.write(m_lastSource[idx].toAscii());
        m_filePath[idx] = filePath;
    }
    f.close();

    return ret;
}

bool SLShaderCodes::openShader(ShaderLab::Shader shaderType, const QString& filePath)
{
    unsigned int idx = ShaderLab::shaderToIndex(shaderType);

    QFile f(filePath);
    bool ret = f.open(QIODevice::ReadOnly);
    if(ret)
    {
        m_lastSource[idx] = QString(f.readAll());
        m_active[idx] = true;
        m_filePath[idx] = filePath;
    }
    f.close();
    return ret;
}

void SLShaderCodes::clearShaders()
{
    for(int i = 0; i < m_shaders.size(); ++i)
    {
        SLShader* s = m_shaders[i];
        if(s)
            delete s;
        m_shaders[i] = NULL;
    }
}

void SLShaderCodes::compile()
{
    m_log = QString();
    m_successfulCompiles.clear();
    m_hasCompiledGeometry = false;

    for(int i = 0; i < m_active.size(); ++i)
    {
        if(m_active[i])
        {
            if(m_shaders[i] != NULL)
                delete m_shaders[i];

            ShaderLab::Shader type = ShaderLab::indexToShader(i);

            m_shaders[i] = new SLShader(type);
            if( m_shaders[i]->compile(m_lastSource[i]) )
            {
                m_successfulCompiles.append(m_shaders[i]->shaderId());
                if(type == ShaderLab::Geometry)
                    m_hasCompiledGeometry = true;
            }
            m_log += m_shaders[i]->log();
        }
    }
}

bool SLShaderCodes::hasCompiledGeometry() const
{
    return m_hasCompiledGeometry;
}

const QString& SLShaderCodes::log()const
{
    return m_log;
}

bool SLShaderCodes::isNew(ShaderLab::Shader shaderType)const
{
    unsigned int idx = ShaderLab::shaderToIndex(shaderType);
    return m_filePath[idx] == ".";
}

QString SLShaderCodes::fileName(ShaderLab::Shader shaderType)const
{
    unsigned int idx = ShaderLab::shaderToIndex(shaderType);
    if(m_filePath[idx] == ".")
        return "new_" + ShaderLab::shaderToStr(shaderType);
    else
        return SLFile::getFileName(m_filePath[ShaderLab::shaderToIndex(shaderType)]);
}

const QString& SLShaderCodes::filePath(ShaderLab::Shader shaderType)const
{
    return m_filePath[ShaderLab::shaderToIndex(shaderType)];
}

bool SLShaderCodes::isChanged(ShaderLab::Shader shaderType)
{
    if(filePath(shaderType).isEmpty())
        return false;
    if(isNew(shaderType))
        return true;

    QFile f(filePath(shaderType));
    bool ret = f.open(QIODevice::ReadOnly);
    QString code;
    if(ret)
    {
        code = QString(f.readAll());
    }
    f.close();

    return code != lastSource(shaderType);
}
