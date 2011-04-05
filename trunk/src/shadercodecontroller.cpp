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
    QString output = "";

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
        output += "======== vertex =========\n" + vshader->log();
    }

    if(! fcode.isEmpty())
    {
        fshader = new QGLShader(QGLShader::Fragment, this);
        fCompiled = fshader->compileSourceCode(fcode);
        output += "======= fragment ========\n" + fshader->log();
    }

    if((!vCompiled) && (!fCompiled))
    {
        mainWindow->setOutputText(output);
        qDebug() << "Ih cagou.....";
        return;
    }

    if(vCompiled)
        program->addShader(vshader);
    if(fCompiled)
        program->addShader(fshader);

    program->link();
    program->bind();

    output += "======== linker =========\n" + program->log();


    mainWindow->setOutputText(output);
    mainWindow->updateDisplay();
}

