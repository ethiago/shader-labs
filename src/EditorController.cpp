#include "EditorController.h"
#include "SLFile.h"
#include "SLCodeContainer.h"
#include "InterfaceRequests.h"

EditorController::EditorController(ShaderLab::Shader shadertype,
                                   const QString& filepath, QObject *parent) :
    QObject(parent)
{
    file = new SLFile(shadertype, filepath);
    m_codeContainer = new SLCodeContainer(shadertype, file->getFileName());
    m_codeContainer->setText(file->getFileContent());

    connect(m_codeContainer, SIGNAL(closeRequestSignal()),
            this, SLOT(slot_closeShaderCode()));

    connect(m_codeContainer, SIGNAL(save()),
            this, SLOT(slot_saveFile()));

    connect(m_codeContainer, SIGNAL(saveAs()),
            this, SLOT(saveAs()));

    connect(m_codeContainer, SIGNAL(textChanged()),
            this, SLOT(fileChanged()));

    connect(m_codeContainer, SIGNAL(activateStatusChanged()),
            this, SLOT(changeTabActivationStatus()));

    if(filepath.isEmpty())
        m_codeContainer->textChanged(true);
}

EditorController::~EditorController()
{
    delete file;

    m_codeContainer->close();
    delete m_codeContainer;
}

void EditorController::slot_closeShaderCode()
{
    if(closeShaderCode())
        emit useless(this);
}

bool EditorController::closeShaderCode()
{

    if( (file->IsNew() || file->getChanged()) )
    {
        InterfaceRequests::OperationState st =
                InterfaceRequests::saveRequestDialog(file->getFileName(), file->IsNew());

        if(st == InterfaceRequests::Yes)
        {
            if(!saveFile())
                return false;
        }else if(st == InterfaceRequests::Cancel)
            return false;
    }

    m_codeContainer->close();

    return true;
}

void EditorController::fileChanged()
{
    file->setChanged(true);
    m_codeContainer->setTabTitle("*"+file->getFileName());
}

void EditorController::slot_saveFile()
{
    saveFile();
}

void EditorController::saveAs()
{
    saveFile(true);
}

bool EditorController::saveFile(bool forceNew)
{
    if( file->IsNew() || forceNew)
    {
        QString filepath = InterfaceRequests::saveAsRequestDialog( file->shaderType() );

        if(filepath.isEmpty()) return false;

        file->setFilePath(filepath);

        file->setChanged(true);
    }

    if( file->getChanged() )
    {
        file->save(m_codeContainer->getText());
        m_codeContainer->setTabTitle(file->getFileName());
    }

    return true;
}

void EditorController::changeTabActivationStatus()
{
    bool active = !file->isActive();
    file->setActive(active);
}

SLCodeContainer * EditorController::codeContainer(void)
{
    return m_codeContainer;
}

const SLFile& EditorController::getFile()
{
    return *file;
}

bool EditorController::isActive()
{
    return file->isActive();
}

QString EditorController::getContent()
{
    return m_codeContainer->getText();
}

QString EditorController::getContentFile()
{
    return file->getFileContent();
}
