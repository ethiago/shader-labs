#include "filecontroller.h"
#include "shadercodecontroller.h"


FileController::FileController(MainWindow *v) :
    QObject(v), view(v)
{
    connect(view, SIGNAL(selectedFile(const QString&, ShaderLab::Shader)), this, SLOT(openFile(const QString&, ShaderLab::Shader)));
    connect(view, SIGNAL(closeTabRequest(ShaderLab::Shader)), this, SLOT(closeShader(ShaderLab::Shader)));

    view->setVisibleVertexTab(false);
    view->setVisibleFragmentTab(false);

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
            view->setVertexCode(file.readAll());
            view->setVisibleVertexTab(true);
            vertexOpened = true;
            break;
        case ShaderLab::Fragment:
            view->setFragmentCode(file.readAll());
            view->setVisibleFragmentTab(true);
            fragmentOpened = true;
            break;
    }

    file.close();
}

void FileController::closeShader(ShaderLab::Shader shader)
{
    if(shader == ShaderLab::Vertex)
    {
        view->setVisibleVertexTab(false);
        vertexOpened = false;
    }else if(shader == ShaderLab::Fragment)
    {
        view->setVisibleFragmentTab(false);
        fragmentOpened = false;
    }
}

QString FileController::getVertexCode(void)
{
    if(vertexOpened)
        return view->vertexCode();
    else
        return QString();
}

QString FileController::getFragmentCode(void)
{
    if(fragmentOpened)
        return view->fragmentCode();
    else
        return QString();
}

