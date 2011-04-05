#include "filecontroller.h"
#include "shadercodecontroller.h"


FileController::FileController(MainWindow *v) :
    QObject(v), view(v)
{
    connect(view, SIGNAL(selectedFile(const QString&, ShaderLab::Shader)), this, SLOT(openFile(const QString&, ShaderLab::Shader)));
    connect(view, SIGNAL(closeTabRequest(ShaderLab::Shader)), this, SLOT(closeShader(ShaderLab::Shader)));

    view->setVisibleShader(false, Vertex);
    view->setVisibleShader(false, Fragment);

    vertexOpened = false;
    fragmentOpened = false;
}

void FileController::openFile(const QString& filename, ShaderLab::Shader shader)
{
    QFile file(filename);
    if(!file.open(QFile::ReadOnly))
        return;

    switch(shader)
    {
        case ShaderLab::Vertex:
            view->setShaderCode(file.readAll(), Vertex);
            view->setVisibleShader(true, Vertex);
            vertexOpened = true;
            break;
        case ShaderLab::Fragment:
            view->setShaderCode(file.readAll(), Fragment);
            view->setVisibleShader(true, Fragment);
            fragmentOpened = true;
            break;
    }

    file.close();
}

void FileController::closeShader(ShaderLab::Shader shader)
{
    view->setVisibleShader(false, shader);

    if(shader == ShaderLab::Vertex)
        vertexOpened = false;
    else if(shader == ShaderLab::Fragment)
        fragmentOpened = false;
}

QString FileController::getVertexCode(void)
{
    if(vertexOpened)
        return view->shaderCode(Vertex);
    else
        return QString();
}

QString FileController::getFragmentCode(void)
{
    if(fragmentOpened)
        return view->shaderCode(Fragment);
    else
        return QString();
}

