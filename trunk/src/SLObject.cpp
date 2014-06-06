#include "SLObject.h"
#include "Object3D.h"
#include "slShaderProgram2.h"
#include "slShaderCodes.h"
#include "slTexture2.h"
#include "Project.h"
#include "slhemesh.h"

SLObject::SLObject(Object3D *obj) :
    QObject(NULL), m_program(new SLShaderProgram2), m_object(obj->copy()),
    m_textures(new SLTextures2), m_shaderCodes(new SLShaderCodes),
    m_project(new Project()), m_visible(true), m_color(QColor(255,255,255)),
    m_wireframe(false)
{

}

SLObject::~SLObject()
{
    delete m_program;
    delete m_textures;

    delete m_object;
    delete m_shaderCodes;
}

void SLObject::changeObject(Object3D *obj)
{
    delete m_object;

    m_object = obj->copy();
}

bool SLObject::isVisible()const
{
    return m_visible;
}

void SLObject::setVisible(bool v)
{
    m_visible = v;
}

void SLObject::setColor(const QColor& color)
{
    m_color = color;
}

const QColor& SLObject::color()const
{
    return m_color;
}

void SLObject::draw()
{
    if(m_visible)
    {
        m_program->bind();

        m_textures->bind();

        if(m_wireframe)
            glPolygonMode(GL_FRONT_AND_BACK,GL_LINE);
        else
            glPolygonMode(GL_FRONT_AND_BACK,GL_FILL);

        m_object->draw(m_color);

        m_textures->release();

        m_program->release();
    }
}

void SLObject::toggleWireframe()
{
    m_wireframe = !m_wireframe;
}

SLShaderProgram2 * SLObject::shaderProgram()
{
    return m_program;
}

SLShaderCodes * SLObject::shaderCodes()
{
    return m_shaderCodes;
}

SLTextures2* SLObject::textures()
{
    return m_textures;
}

Project *SLObject::project()
{
    return m_project;
}

int SLObject::modelId()const
{
    return m_object->modelId();
}

Object3D* SLObject::object()const
{
    return m_object;
}
