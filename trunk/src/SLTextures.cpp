#include "SLTextures.h"
#include "Texture3D.h"
#include "TexturePropertiesView.h"
#include "Texture.h"
#include "MainWindow.h"
#include <QImage>
#include <QFileDialog>
#include <QAction>
#include "InterfaceRequests.h"
#include "SLTexture3dDialog.h"
#include "SLShaderProgram.h"
#include <QGLWidget>

SLTextures::SLTextures(TexturePropertiesView* tpv, SLTexture3DDialog * t3d, QObject* parent) :
    QObject(parent), m_textureView(tpv), m_texture3DDialog(t3d)
{
   m_textureList.append(Texture());

   connect(m_textureView, SIGNAL(loadTextureClicked()),
           this, SLOT(changeTexture()));

   connect(m_textureView, SIGNAL(removeTextureClicked()),
           this, SLOT(removeTexture()));

   connect(m_textureView, SIGNAL(addTextureClicked()),
           this, SLOT(addTexture()));

   connect(m_textureView, SIGNAL(textureCurrentChange(int)),
           this,SLOT(textureCurrentChange(int)));

   connect(m_textureView, SIGNAL(addTexture3DClicked()),
           this, SLOT(addTexture3D()));

   textureCurrentChange(0);
   viewUpdateList();
}

SLTextures::~SLTextures()
{
    disconnect(this);
}

void SLTextures::changeTexture(void)
{
    QString filename = InterfaceRequests::openTexture();

    setupTexture(filename, false);
}

void SLTextures::addTexture(void)
{
    QString filename = InterfaceRequests::openTexture();

    setupTexture(filename, true);
}

void SLTextures::setupTexture(const QString& imageFileName, bool add)
{
    ShaderLab *sl = ShaderLab::instance();

    if(imageFileName.isEmpty())
        return;

    if(!Texture::isValid(imageFileName))
    {
        InterfaceRequests::openFileProblem(imageFileName);
        return;
    }

    if(add && m_textureList[0].glTextureName() >= 0)
    {
            m_textureList.push_back(Texture());
            textureContext = m_textureList.size() - 1;
    }

    QImage img(imageFileName);

    clearTexture();

    m_textureList[textureContext].setImage(img);
    m_textureList[textureContext].setGLTextureName(sl->glContext()->bindTexture(imageFileName, GL_TEXTURE_2D));
    m_textureList[textureContext].setVarName(SAMPLEPREFIX2D + QString::number(textureContext));
    m_textureList[textureContext].setFileName(imageFileName);
    activeTextures();
    viewUpdateList();
    sl->glContext()->updateGL();
}

void SLTextures::clearTexture(void)
{
    ShaderLab *sl = ShaderLab::instance();
    if(m_textureList[textureContext].glTextureName() >= 0)
    {

        sl->glContext()->deleteTexture(m_textureList[textureContext].glTextureName());
        m_textureList[textureContext].clean();
    }
}

void SLTextures::remakeVarNames()
{
    for(int i = 0; i < m_textureList.size(); i++)
    {
            m_textureList[i].setVarName(SAMPLEPREFIX2D + QString::number(i));
    }
}

void SLTextures::removeTexture(void)
{

    ShaderLab *sl = ShaderLab::instance();
    clearTexture();

    if(m_textureList.size() > 1)
    {
        m_textureList.removeAt(textureContext);
        remakeVarNames();
        if(textureContext == m_textureList.size())
            textureCurrentChange(textureContext-1);
        else
            textureCurrentChange(textureContext);
    }else
    {
        textureCurrentChange(textureContext);
    }

    viewUpdateList();
    activeTextures();
    sl->glContext()->updateGL();
}

void SLTextures::textureCurrentChange(int index)
{
    ShaderLab *sl = ShaderLab::instance();
    if(index < 0)
        return;

    textureContext = index;
    m_textureView->setTexture(m_textureList[textureContext]);
    activeTextures();
    sl->glContext()->updateGL();
}

void SLTextures::viewUpdateList(void)
{
    QList<QPair<QIcon, QString> > list;
    for(int i = 0; i < m_textureList.size(); ++i)
    {
        QPair<QIcon, QString> pair;
        pair.first = QIcon(QPixmap::fromImage(m_textureList[i].image()));
        pair.second = m_textureList[i].varName();
        list.push_back(pair);
    }

    m_textureView->setTextureList(list, textureContext);
}

void SLTextures::setUniformTexture(SLShaderProgram* program)
{
    for(int i = 0; i < m_textureList.size(); ++i)
    {
        program->setUniformValue(m_textureList[i].uniformId(), (GLint)i );
    }
}

void SLTextures::setupUniformTextureNames(SLShaderProgram* program)
{
    for(int i = 0; i < m_textureList.size(); ++i)
    {
        const char * name = m_textureList[i].varName().toAscii();
        m_textureList[i].setUniformId(program->uniformLocation(name));
    }
}

void SLTextures::activeTextures(void)
{
    for(int i = 0; i < m_textureList.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, m_textureList[i].glTextureName());
    }
}

QStringList SLTextures::getTextureFileNames()
{
    QStringList filenames;

    for(int i = 0; i < m_textureList.size(); ++i)
    {
        QString filename = m_textureList[i].fullFileName();
        if(!filename.isEmpty())
        {
            filenames.append(filename);
        }
    }

    return filenames;
}

void SLTextures::removeAllTextures(void)
{
    textureContext = m_textureList.size() - 1;

    while(textureContext > 0)
        removeTexture();

    clearTexture();
}

void SLTextures::setTextures(const QStringList& list)
{
    removeAllTextures();

    for(int i = 0; i < list.size(); ++i)
    {
        setupTexture(list[i], true);
    }

    viewUpdateList();
    activeTextures();
    ShaderLab::instance()->glContext()->updateGL();
}
