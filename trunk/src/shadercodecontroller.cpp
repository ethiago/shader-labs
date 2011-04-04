#include "shadercodecontroller.h"
#include "filecontroller.h"
#include <QGLShader>
#include <QDebug>

ShaderCodeController::ShaderCodeController(FileController *fc) :
    QObject(fc), fileController(fc)
{
}

void ShaderCodeController::runShaderCode(ShaderLab::Shader shader)
{
    QString code;
    QGLShaderProgram *program;
    QGLShader *vshader;

    switch(shader)
    {
        case Vertex:
            code = fileController->getVertexCode();
            vshader = new QGLShader(QGLShader::Vertex, this);

            vshader->compileSourceCode(code);

            qDebug() << vshader->log();

            program = new QGLShaderProgram(this);
            program->addShader(vshader);

            //program->bindAttributeLocation("vertex", PROGRAM_VERTEX_ATTRIBUTE);
            //program->bindAttributeLocation("texCoord", PROGRAM_TEXCOORD_ATTRIBUTE);
            program->link();

            program->bind();
            program->setUniformValue("texture", 0);
            break;

    case Fragment:
            break;
    }

}
