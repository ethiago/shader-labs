#include "SLObject2.h"
#include "Project.h"
#include "InterfaceRequests.h"
#include "MainWindow.h"
#include "Object3D.h"
#include "SLShaderProgram.h"
#include "SLTextures.h"

SLObject2::SLObject2(MainWindow *mw,  SLShaderProgram* slp, SLTextures* slt, QObject *parent) :
    QObject(parent), m_shader(slp), m_object(NULL), m_textures(slt), m_project(NULL)
{
    connect(mw, SIGNAL(runShaders()), this, SLOT(compileShaders()));
    connect(this, SIGNAL(sendLog(QString)), mw, SLOT(shaderLog(QString)));
}

SLObject2::~SLObject2()
{
    if(m_project)
        delete m_project;
}

void SLObject2::deleteObject3D()
{
    if(m_object)
        delete m_object;
}

void SLObject2::setObject(Object3D *obj)
{
    m_object = obj;
    m_shader->setGeometryInputType(obj->inputType());
}
/*
SLShader* SLObject2::shader()
{
    return m_shader;
}
*/

void SLObject2::draw()
{
    m_textures->activateTexture();
    if(m_shader->isLinked())
    {
        m_textures->applyTextures(m_shader);
        m_shader->bind();

        m_shader->setUniformValue("wsize", ShaderLab::instance()->glContext()->size());
    }

    m_object->draw();
    m_shader->release();
}

void SLObject2::compileShaders()
{
   m_shader->compileAndLink();
   emit sendLog(m_shader->log());
   ShaderLab::instance()->glContext()->updateGL();
}

QString SLObject2::shaderLog()
{
    return m_shader->log();
}

void SLObject2::setTexturesFromProject(const QStringList& list)
{
   m_textures->setTextures(list);
}

void SLObject2::closeProject()
{
    if(m_project)
        delete m_project;
    m_project = NULL;
}

void SLObject2::setProject(Project *p)
{
    if(m_project)
        delete m_project;
    m_project = p;
}

void SLObject2::close()
{
    closeProject();

    m_object = m_object->copy();

    disconnect(this);
    disconnect(this, 0, 0, 0);
}

QString SLObject2::saveMerge(bool as)
{
    QString projectFileName;
    if(as)
    {
        closeProject();
    }else
    {
        if(m_project == NULL)
        {
            if(InterfaceRequests::createProject())
                as = true;
            else
                return QString();
        }
    }
    m_shader->saveAllShaders();
    if(m_shader->isAnyNew())
    {
        if(!InterfaceRequests::projectSaveContinue())
            return QString();
    }

    if(as)
    {
       projectFileName = InterfaceRequests::saveProjectAsRequestDialog();
       if(projectFileName.isEmpty())
           return QString();

        m_project = new Project;
    }
    else
        projectFileName = QString();


    FORENABLEDSHADERS(shaderType)
    {
        QString filepath = m_shader->getAbsoluteFilePath(shaderType);
        m_project->checkShader(filepath, shaderType);
    }

    m_project->setModel(m_object->modelId());
    m_project->setTextures(m_textures->getTextureFileNames());

    m_project->save(projectFileName);

    return m_project->getAbsoluteFilePath();
}


SLTextures * SLObject2::textures()
{
    return m_textures;
}
