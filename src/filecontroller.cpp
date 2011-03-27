#include "filecontroller.h"


FileController::FileController(MainWindow *v) :
    QObject(v), view(v)
{
    connect(view, SIGNAL(selectedFile(const QString&, ShaderLab::Shader)), this, SLOT(openFile(const QString&, ShaderLab::Shader)));

    view->setVisibleVertexTab(false);
    view->setVisibleFragmentTab(false);
}

void FileController::openFile(const QString& filename, ShaderLab::Shader shader)
{
    QFile file(filename);
    file.open(QFile::ReadOnly);

    switch(shader)
    {
        case ShaderLab::Vertex:
            view->setVertexCode(file.readAll());
            view->setVisibleVertexTab(true);
            break;
        case ShaderLab::Fragment:
            view->setFragmentCode(file.readAll());
            view->setVisibleFragmentTab(true);
            break;
    }

    file.close();

}
