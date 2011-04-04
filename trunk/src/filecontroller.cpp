#include "filecontroller.h"
#include "shadercodecontroller.h"


FileController::FileController(MainWindow *v) :
    QObject(v), view(v)
{
    connect(view, SIGNAL(selectedFile(const QString&, ShaderLab::Shader)), this, SLOT(openFile(const QString&, ShaderLab::Shader)));
    connect(view, SIGNAL(closeTabRequest(ShaderLab::Shader)), this, SLOT(closeShader(ShaderLab::Shader)));

    view->setVisibleVertexTab(false);
    view->setVisibleFragmentTab(false);
}

void FileController::openFile(const QString& filename, ShaderLab::Shader shader)
{
    QFile file(filename);
    ShaderCodeController sc(this);
    file.open(QFile::ReadOnly);

    switch(shader)
    {
        case ShaderLab::Vertex:
            view->setVertexCode(file.readAll());
            view->setVisibleVertexTab(true);
            sc.runShaderCode(ShaderLab::Vertex);
            break;
        case ShaderLab::Fragment:
            view->setFragmentCode(file.readAll());
            view->setVisibleFragmentTab(true);
            break;
    }

    file.close();



}

void FileController::closeShader(ShaderLab::Shader shader)
{
    if(shader == ShaderLab::Vertex)
        view->setVisibleVertexTab(false);
    else if(shader == ShaderLab::Fragment)
        view->setVisibleFragmentTab(false);
}

QString FileController::getVertexCode(void)
{
    return view->vertexCode();
}
