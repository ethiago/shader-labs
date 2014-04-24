#include "SLObjectController.h"
#include "MainWindow.h"
#include "TexturePropertiesView.h"
#include "SLObject2.h"
#include "PrimitivesDialog.h"
#include "SLShaderProgramController.h"
#include "SLShaderProgram.h"
#include "SLTextures.h"

SLObjectController::SLObjectController(MainWindow* mw, QObject *parent) :
    QObject(parent), mainWindow(mw), object(NULL)
{
    m_texturePropertiesView = new TexturePropertiesView(mainWindow);
    m_primitivesDialog = new PrimitivesDialog(mainWindow);
    m_programController = new  SLShaderProgramController(mainWindow, m_primitivesDialog, this);
}

SLObjectController::~SLObjectController()
{
    closeObject();

    delete m_programController;
    delete m_primitivesDialog;
    delete m_texturePropertiesView;  
}

SLObject2 * SLObjectController::newObject(Object3D* obj)
{
    closeObject();

    SLShaderProgram* shaderProgram = new SLShaderProgram();
    SLTextures * textures = new SLTextures(m_texturePropertiesView, this);
    m_programController->setShaderProgram(shaderProgram);
    object = new SLObject2(mainWindow, shaderProgram, textures);
    object->setObject(obj);
    return object;
}

SLShaderProgramController* SLObjectController::programController()
{
    return m_programController;
}

void SLObjectController::setTexturesFromProject(const QStringList& list)
{
    object->setTexturesFromProject(list);
}

QString SLObjectController::saveProject(bool as)
{
    return object->saveMerge(as);
}

void SLObjectController::closeObject()
{
    if(object != NULL)
    {
        SLTextures * textures = object->textures();
        disconnect(m_texturePropertiesView, 0, textures, 0);
        disconnect(textures, 0, m_texturePropertiesView, 0);
        object->close();
    }
}
