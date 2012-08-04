#include "SLObject.h"
#include "Project.h"
#include "InterfaceRequests.h"
#include "MainWindow.h"

SLObject::SLObject(MainWindow *mw,  QObject *parent) :
    QObject(parent), m_shader(new SLShader(mw)), m_object(NULL),
    textures(0), m_project(NULL)
{
    connect(mw, SIGNAL(runShaders()), this, SLOT(compileShaders()));
    connect(this, SIGNAL(sendLog(QString)), mw, SLOT(shaderLog(QString)));
}

SLObject::~SLObject()
{
    if(m_project)
        delete m_project;
    if(m_shader)
        delete m_shader;
}

void SLObject::deleteObject3D()
{
    if(m_object)
        delete m_object;
}

void SLObject::setObject(Object3D *obj)
{
    m_object = obj;
}
SLShader* SLObject::shader()
{
    return m_shader;
}

void SLObject::draw()
{
    textures.activateTexture();
    if(program.isLinked())
    {
        textures.applyTextures(&program);
        program.bind();

        program.setUniformValue("wsize", ShaderLab::instance()->glContext()->size());
    }

    m_object->draw();
    program.release();
}

void SLObject::compileShaders()
{
   m_shader->compileAndLink(&program, m_object);
   emit sendLog(m_shader->log());
   ShaderLab::instance()->glContext()->updateGL();
}

const QString& SLObject::shaderLog()
{
    return m_shader->log();
}

void SLObject::setTexturesFromProject(const QStringList& list)
{
   textures.setTextures(list);
}

void SLObject::closeProject()
{
    if(m_project)
        delete m_project;
    m_project = NULL;
}

void SLObject::setProject(Project *p)
{
    if(m_project)
        delete m_project;
    m_project = p;
}

void SLObject::close(MainWindow* mw)
{
    closeProject();
    delete m_shader;
    m_shader = NULL;

    m_object = m_object->copy();

    textures.closeView(mw);

    disconnect(mw, 0, this, 0);
    disconnect(this, 0, mw, 0);
}

QString SLObject::saveMerge(bool as)
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
    m_project->setTextures(textures.getTextureFileNames());

    m_project->save(projectFileName);

    return m_project->getAbsoluteFilePath();
}
