#include "EditorController.h"
#include "SLFile.h"
#include "SLCodeContainer.h"
#include "InterfaceRequests.h"

EditorController::EditorController(const QString& filepath,
                                   ShaderLab::Shader shadertype, QObject *parent) :
    QObject(parent)
{
    file = new SLFile(filepath, shadertype);
    codeContainer = new SLCodeContainer(shadertype);

    connect(codeContainer, SIGNAL(closeRequestSignal()),
            this, SLOT(slot_closeShaderCode()));

    connect(codeContainer, SIGNAL(save()),
            this, SLOT(slot_saveFile()));

    connect(codeContainer, SIGNAL(textChanged()),
            this, SLOT(fileChanged()));

    connect(codeContainer, SIGNAL(activateStatusChanged()),
            this, SLOT(changeTabActivationStatus()));
}

EditorController::~EditorController()
{
    if(file)
        delete file;
    if(codeContainer)
    {
        codeContainer->close();
        delete codeContainer;
    }
}

void EditorController::slot_closeShaderCode()
{
    closeShaderCode();
}

bool EditorController::closeShaderCode()
{

    if( (file->IsNew() || file->getChanged()) )
    {
        InterfaceRequests::OperationState st =
                InterfaceRequests::saveRequestDialog(file->getFileName(), file->IsNew());

        if(st == InterfaceRequests::Yes)
            saveFile();
        else if(st == InterfaceRequests::Cancel)
            return false;
    }

    codeContainer->close();

    delete file;
    delete codeContainer;

    return true;

}

void EditorController::fileChanged()
{
    file->setChanged(true);
    codeContainer->setWindowTitle("*"+file->getFileName());
}

void EditorController::slot_saveFile()
{
    saveFile();
}

bool EditorController::saveFile()
{
    if( file->IsNew() )
    {
        QString filepath = InterfaceRequests::saveAsRequestDialog( file->shaderType() );

        if(filepath.isEmpty()) return false;

        file->setFilePath(filepath);
    }

    if( file->getChanged() )
    {
        file->save(codeContainer->getText());
        codeContainer->setWindowTitle(file->getFileName());
    }

    return true;
}

void EditorController::changeTabActivationStatus()
{
    bool active = !file->isActive();
    file->setActive(active);
}
