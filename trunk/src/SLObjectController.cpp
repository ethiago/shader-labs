#include "SLObjectController.h"
#include "MainWindow.h"
#include "TexturePropertiesView.h"
#include "SLObject2.h"
#include "SLTexture3dDialog.h"
#include "PrimitivesDialog.h"
#include "SLShaderProgramController.h"
#include "SLShaderProgram.h"
#include "SLTextures.h"

SLObjectController::SLObjectController(MainWindow* mw, QObject *parent) :
    QObject(parent), mainWindow(mw), object(NULL)
{
    m_texturePropertiesView = new TexturePropertiesView(mainWindow);
    m_texture3dDialog = new SLTexture3DDialog(mainWindow);
    m_texture3dDialog->setModal(true);
    m_primitivesDialog = new PrimitivesDialog(mainWindow);
    m_programController = new  SLShaderProgramController(mainWindow, m_primitivesDialog, this);
}

SLObjectController::~SLObjectController()
{
    delete m_texturePropertiesView;
    delete m_texture3dDialog;
    delete m_primitivesDialog;
    delete m_programController;
}

SLObject2 * SLObjectController::newObject(Object3D* obj)
{
    closeObject();

    SLShaderProgram* shaderProgram = new SLShaderProgram(this);
    SLTextures * textures = new SLTextures(m_texturePropertiesView,m_texture3dDialog, this);
    m_programController->setShaderProgram(shaderProgram);
    object = new SLObject2(mainWindow, shaderProgram, textures, this );
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
        disconnect(m_texture3dDialog, 0, textures, 0);
        disconnect(textures, 0, m_texture3dDialog, 0);
        object->close();
    }
}
