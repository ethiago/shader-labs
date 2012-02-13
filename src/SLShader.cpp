#include "SLShader.h"

#define DEFFRAG "void main (){gl_FragColor = gl_Color;}"

SLShader::SLShader(QObject *parent) :
    QObject(parent), vertexShader(ShaderLab::Vertex),
    geometryShader(ShaderLab::Geometry), fragmentShader(ShaderLab::Fragment)
{
    vertexShader.setActive(false);
    geometryShader.setActive(false);
    fragmentShader.setActive(false);
}

void SLShader::logProcess(const SLFile& shader)
{
    m_log += "==================== Compiling "+ ShaderLab::shaderToStr(shader.shaderType()) +" code ====================\n";
    QString tlog = shader.log();

    if(tlog == "")
        m_log += "Successfull.\n";
    else
        m_log += tlog + "\n";
}

void SLShader::compileAndLink(GLenum inputType, GLenum outputType)
{
    program.removeAllShaders();
    program.release();

    bool atLeastOne = false;
    bool compOK, thereIsCode = false;
    bool fragmentOk = false;

    m_log = "";

    if(vertexShader.isActive())
    {
        thereIsCode = true;
        compOK = vertexShader.compile();

        if(compOK)
        {
            program.addShader(vertexShader.getShader());

            atLeastOne = true;
        }
        logProcess(vertexShader);
    }

    if(geometryShader.isActive())
    {
        thereIsCode = true;
        compOK = geometryShader.compile();

        if(compOK)
        {
            program.addShader(geometryShader.getShader());

            atLeastOne = true;
        }
        logProcess(geometryShader);
    }

    if(fragmentShader.isActive())
    {
        thereIsCode = true;
        compOK = fragmentShader.compile();

        if(compOK)
        {
            program.addShader(fragmentShader.getShader());

            atLeastOne = true;
            fragmentOk = true;
        }
        logProcess(fragmentShader);
    }

    if(atLeastOne && !fragmentOk)
    {
        program.addShaderFromSourceCode(QGLShader::Fragment, QString(DEFFRAG));
    }

    m_log += "====================== Linking process ======================\n";
    if(atLeastOne)
    {

        ShaderLab * sl = ShaderLab::instance();
        if(sl->geometryShaderEnabled())
        {
            program.setGeometryInputType(inputType);
            program.setGeometryOutputType(outputType);
        }

        program.link();
        m_log += program.log();
    }
    else
        m_log += "Due to problems, linking process could not be performed.";

    if(!thereIsCode)
        m_log = tr("No active shader code to compile.");
}

const QString& SLShader::log()
{
    return m_log;
}

void SLShader::release()
{
    program.release();
}

void SLShader::bind()
{
    program.bind();
}

void SLShader::setShader(const QString& filePath, ShaderLab::Shader shadertype)
{
    if(shadertype == ShaderLab::Vertex)
    {
        vertexShader.setFilePath(filePath);
        vertexShader.setActive(true);
    }
}
