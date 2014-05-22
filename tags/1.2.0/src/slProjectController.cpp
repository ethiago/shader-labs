#include "slProjectController.h"
#include "SLObject.h"
#include "slShaderCodes.h"
#include "slTexture2.h"
#include "Project.h"
#include "InterfaceRequests.h"
#include "slTextureController.h"
#include "slshadercontroller.h"
#include "Scene3D.h"
#include "rendercontroller2.h"

#include <QDebug>

SLProjectController::SLProjectController(Scene3D *scene,
                                         SLShaderController * shaderController,
                                         SLTextureController* textureController,
                                         RenderController2* renderController) :
    QObject(NULL), e_scene(scene), e_shaderController(shaderController),
    e_textureController(textureController), e_renderController(renderController)
{
}

SLProjectController::~SLProjectController()
{
}

void SLProjectController::currentChanged()
{
    SLObject *obj = e_scene->currentSLObject();
    if(obj == NULL)
    {
        emit projectNameChanged("");
    }

    Project * p = obj->project();
    if(p->isOpened())
    {
        emit projectNameChanged(p->getAbsoluteFilePath());
    }else
        emit projectNameChanged("");
}

bool SLProjectController::isOpened()const
{
    SLObject *obj = e_scene->currentSLObject();
    if(obj == NULL)
        return false;

    Project * p = obj->project();
    return p->isOpened();
}

bool SLProjectController::open()
{
    SLObject *obj = e_scene->currentSLObject();
    if(obj == NULL)
        return false;

    Project * p = obj->project();
    if(p->isOpened())
    {
        InterfaceRequests::thereIsAlreadyAnOpenedProject();
        return false;
    }

    if(!e_shaderController->canOpenProject())
    {
        InterfaceRequests::openedCode_Project();
        return false;
    }

    QString fn = InterfaceRequests::openProject();
    if(fn.isEmpty())
        return false;

    if(!p->load(fn))
    {
        p->close();
        return false;
    }

    e_shaderController->projectOpened(p);
    e_textureController->projectOpened(p);
    e_renderController->projectOpened(p);

    emit projectNameChanged(p->getAbsoluteFilePath());

    ShaderLab::instance()->glContext()->updateGL();
    return true;
}

bool SLProjectController::checkNewFile()
{
    SLObject *obj = e_scene->currentSLObject();
    if(obj == NULL)
        return true;

    SLShaderCodes * codes = obj->shaderCodes();
    FORENABLEDSHADERS(shaderType)
    {
        QString fn  = codes->filePath(shaderType);
        if(fn == ".")
            return true;
    }

    return false;
}

bool SLProjectController::save()
{
    SLObject *obj = e_scene->currentSLObject();
    if(obj == NULL)
        return true;

    if(checkNewFile())
    {
        InterfaceRequests::notSavedCode();
        return false;
    }

    Project * p = obj->project();
    if(!p->isOpened())
        return saveAs();

    SLTextures2 * tex = obj->textures();
    SLShaderCodes * codes = obj->shaderCodes();

    FORENABLEDSHADERS(shaderType)
    {
        QString fn = codes->filePath(shaderType);
        Project::FileStatus fs = p->checkShader(fn, shaderType);
        InterfaceRequests::OperationState st;
        switch (fs) {
        case Project::OK:
            break;
        case Project::Have:
            st = InterfaceRequests::removeFileFromProject(fn);
            if(st == InterfaceRequests::Cancel)
                return false;
            else if(st == InterfaceRequests::Yes)
                p->removeShader(shaderType);
            break;
        case Project::NotHave:
            st = InterfaceRequests::includeFileIntoProject(fn);
            if(st == InterfaceRequests::Cancel)
                return false;
            else if(st == InterfaceRequests::Yes)
                p->includeShader(fn, shaderType);
            break;
        case Project::IsDifferent:
            st = InterfaceRequests::replaceFileIntoProject(fn);
            if(st == InterfaceRequests::Cancel)
                return false;
            else if(st == InterfaceRequests::Yes)
                p->includeShader(fn, shaderType);
            break;
        }

    }

    p->setTextures( tex->getTextureFileNames() );
    p->setModel( e_renderController->getModelId() );

    return p->save();
}

bool SLProjectController::saveAs()
{
    SLObject *obj = e_scene->currentSLObject();
    if(obj == NULL)
        return true;

    if(checkNewFile())
    {
        InterfaceRequests::notSavedCode();
        return false;
    }

    InterfaceRequests::OperationState st;
    Project * p = obj->project();
    if(p->isOpened())
    {
        st = InterfaceRequests::openedProjectContinueAsk();
        if(st == InterfaceRequests::Cancel)
            return false;
    }

    QString proFile = InterfaceRequests::saveProjectAsRequestDialog();

    SLTextures2 * tex = obj->textures();
    SLShaderCodes * codes = obj->shaderCodes();

    FORENABLEDSHADERS(shaderType)
    {
        QString fn = codes->filePath(shaderType);
        if(fn.isEmpty())
            continue;

        p->includeShader(fn, shaderType);
    }

    p->setTextures( tex->getTextureFileNames() );
    p->setModel( e_renderController->getModelId() );

    if(!p->save(proFile))
    {
        p->close();
        return false;
    }

    emit projectNameChanged(proFile);

    return true;
}

bool SLProjectController::close()
{
    SLObject *obj = e_scene->currentSLObject();
    if(obj == NULL)
        return true;

    SLShaderCodes * prog = obj->shaderCodes();
    Project * p = obj->project();
    if(!p->isOpened())
        return true;

    bool diff = false;
    FORENABLEDSHADERS(shaderType)
    {

        if(p->checkShader(prog->filePath(shaderType), shaderType) !=
                Project::OK)
            diff = true;
    }

    if(diff)
    {
        InterfaceRequests::OperationState st;
        st = InterfaceRequests::projectDifferences();
        if(st == InterfaceRequests::Cancel)
            return false;
        if(st == InterfaceRequests::Yes && !save())
            return false;
    }

    p->close();

    emit projectNameChanged("");

    return true;
}
