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
    QObject(parent), mainWindow(mw), m_object(NULL)
{
    m_texturePropertiesView = new TexturePropertiesView(mainWindow);
    m_texture3dDialog = new SLTexture3DDialog(mainWindow);
    m_texture3dDialog->setModal(true);
    m_primitivesDialog = new PrimitivesDialog(mainWindow);
    m_programController = new  SLShaderProgramController(mainWindow, m_primitivesDialog, this);
}

SLObjectController::~SLObjectController()
{
    if(m_object != NULL)
        delete m_object;
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
    m_object = new SLObject2(mainWindow, shaderProgram, textures, this );
    m_object->setObject(obj);
    return m_object;
}

SLShaderProgramController* SLObjectController::programController()
{
    return m_programController;
}

void SLObjectController::setTexturesFromProject(const QStringList& list)
{
    m_object->setTexturesFromProject(list);
}

QString SLObjectController::saveProject(bool as)
{
    return m_object->saveMerge(as);
}

void SLObjectController::closeObject()
{
    if(m_object != NULL)
    {
        SLTextures * textures = m_object->textures();
        disconnect(m_texturePropertiesView, 0, textures, 0);
        disconnect(textures, 0, m_texturePropertiesView, 0);
        disconnect(m_texture3dDialog, 0, textures, 0);
        disconnect(textures, 0, m_texture3dDialog, 0);
        m_object->close();
    }
}
