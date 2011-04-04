#include "shadercodecontroller.h"
#include "filecontroller.h"
#include <QGLShader>
#include <QDebug>

ShaderCodeController::ShaderCodeController(FileController *fc, MainWindow *w) :
    QObject(fc), fileController(fc), mainWindow(w)
{
    connect(mainWindow, SIGNAL(runShaders()), this, SLOT(runShaderCode()));
}

void ShaderCodeController::runShaderCode(void)
{
    QString vcode;
    QString fcode;

    bool vCompiled = false;
    bool fCompiled = false;

    QGLShaderProgram *program = new QGLShaderProgram(this);
    QGLShader *vshader;
    QGLShader *fshader;

    vcode = fileController->getVertexCode();
    fcode = fileController->getFragmentCode();

    if(! vcode.isEmpty())
    {
        vshader = new QGLShader(QGLShader::Vertex, this);
        vCompiled = vshader->compileSourceCode(vcode);
        qDebug() << "======== vertex =========\n" << vshader->log();
    }

    if(! fcode.isEmpty())
    {
        fshader = new QGLShader(QGLShader::Fragment, this);
        fCompiled = fshader->compileSourceCode(fcode);
        qDebug() << "======= fragment ========\n" << fshader->log();
    }

    if((!vCompiled) && (!fCompiled))
    {
        qDebug() << "Ih cagou.....";
        return;
    }

    if(vCompiled)
        program->addShader(vshader);
    if(fCompiled)
        program->addShader(fshader);

    program->link();
    program->bind();

    mainWindow->updateDisplay();
}

