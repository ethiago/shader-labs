#include "slTextureController.h"
#include "MainWindow.h"
#include "TexturePropertiesView.h"
#include "slTexture2.h"
#include "InterfaceRequests.h"
#include "slShaderProgram2.h"
#include "QGLWidget"
#include "Project.h"

#include <QDebug>
#include <QIcon>

SLTextureController::SLTextureController(MainWindow *mw) : QObject(NULL),
    m_textureView(new TexturePropertiesView(mw)),
    e_textures(NULL)
{
    connect(m_textureView, SIGNAL(loadTextureClicked()),
            this, SLOT(changeTexture()));

    connect(m_textureView, SIGNAL(removeTextureClicked()),
            this, SLOT(removeTexture()));

    connect(m_textureView, SIGNAL(addTextureClicked()),
            this, SLOT(addTexture()));

    connect(m_textureView, SIGNAL(textureCurrentChange(int)),
            this, SLOT(textureCurrentChange(int)));
}

SLTextureController::~SLTextureController()
{
    delete m_textureView;
}

void SLTextureController::setTextures(SLTextures2 * textures)
{
    e_textures = textures;
    updateView();
}

void SLTextureController::beforeUnLink(GLuint programId)
{
}

void SLTextureController::afterLink(GLuint programId)
{
    for(int i = 0; i < e_textures->size(); ++i)
    {
        QString varName = e_textures->at(i).varName();
        int location = SLShaderProgram2::getUniformLocation(programId, varName);
        e_textures->at(i).setUniformLocation(location);
    }
}

void SLTextureController::changeTexture()
{
    if(e_textures->size() > 0)
    {
        QString fn = InterfaceRequests::openTexture();

        if(!fn.isEmpty())
        {
            int idx = m_textureView->current();
            e_textures->setTexture(fn,idx);
            updateView();
        }
    }
}

void SLTextureController::removeTexture()
{
    int idx = m_textureView->current();
    if(idx >= 0)
    {
        e_textures->removeTexture(idx);
        updateView();
    }
}

void SLTextureController::addTexture()
{
    QString fn = InterfaceRequests::openTexture();

    if(!fn.isEmpty())
    {
        int idx = e_textures->setTexture(fn);
        if(idx >= 0)
        {
            Texture t(e_textures->at(idx));
            m_textureView->addItem(t.icon(), t.varName());
        }
    }
}

void SLTextureController::textureCurrentChange(int idx)
{
    if(idx >= 0)
        m_textureView->setTexture(e_textures->at(idx));
    else
        m_textureView->clear();

    ShaderLab::instance()->glContext()->updateGL();

}

void SLTextureController::updateView()
{
    int idx = m_textureView->current();
    m_textureView->clear();
    for(int i = 0; i < e_textures->size(); ++i)
    {
        Texture t(e_textures->at(i));
        m_textureView->addItem(t.icon(), t.varName());
    }
    if(idx >= 0 && idx < e_textures->size())
        m_textureView->setCurrent(idx);
}

void SLTextureController::projectOpened(Project* p)
{
    int qtd = e_textures->size();
    for(int i = 0; i < qtd; ++i)
        removeTexture();

    QStringList texNames = p->getTextures();

    for(int i = 0; i < texNames.size(); ++i)
    {
        QString fn = texNames[i];
        if(!fn.isEmpty())
        {
            int idx = e_textures->setTexture(fn);
            if(idx >= 0)
            {
                Texture t(e_textures->at(idx));
                m_textureView->addItem(t.icon(), t.varName());
            }
        }
    }

}
