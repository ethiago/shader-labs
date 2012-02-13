#include "SLTextures.h"
#include "TexturePropertiesView.h"
#include "Texture.h"
#include "MainWindow.h"
#include <QImage>
#include <QFileDialog>
#include <QAction>
#include "InterfaceRequests.h"

SLTextures::SLTextures(QObject *parent) :
    QObject(parent)
{
   m_textureList.append(Texture());

   m_textureView = new TexturePropertiesView(NULL);

   connect(m_textureView, SIGNAL(loadTextureClicked()),
           this, SLOT(loadTexture()));

   connect(m_textureView, SIGNAL(removeTextureClicked()),
           this, SLOT(removeTexture()));

   connect(m_textureView, SIGNAL(addTextureClicked()),
           this, SLOT(addTexture()));

   connect(m_textureView, SIGNAL(textureCurrentChange(int)),
           this,SLOT(textureCurrentChange(int)));

   textureCurrentChange(0);
   viewUpdateList();

}

void SLTextures::loadTexture(void)
{
    QString filename = QFileDialog::getOpenFileName(m_textureView, "Open Image", ".", tr("Images (*.png *.jpg *.tiff *.svg)"));

    if(!filename.isEmpty())
        textureFileName(filename);
}

void SLTextures::addTexture(void)
{
    if(m_textureList[0].glTextureName() >= 0)
    {
        m_textureList.push_back(Texture());
        textureContext = m_textureList.size() - 1;
    }
    loadTexture();
}

void SLTextures::textureFileName(const QString& imageFileName)
{
    ShaderLab *sl = ShaderLab::instance();
    QImage img(imageFileName);
    if(img.isNull())
    {
        InterfaceRequests::openFileProblem(imageFileName);
        return;
    }

    clearTexture();

    m_textureList[textureContext].setImage(img);
    m_textureList[textureContext].setGLTextureName(sl->glContext()->bindTexture(imageFileName, GL_TEXTURE_2D));
    m_textureList[textureContext].setVarName(SAMPLEPREFIX + QString::number(textureContext));
    m_textureList[textureContext].setFileName(imageFileName);
    activateTexture();
    viewUpdateList();
    emit updateGL();
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
void SLTextures::removeTexture(void)
{

    clearTexture();

    if(m_textureList.size() > 1)
    {
        m_textureList.removeAt(textureContext);
        if(textureContext == m_textureList.size())
            textureCurrentChange(textureContext-1);
        else
            textureCurrentChange(textureContext);
    }else
    {
        textureCurrentChange(textureContext);
    }

    viewUpdateList();
    activateTexture();
    emit updateGL();
}

void SLTextures::textureCurrentChange(int index)
{
    if(index < 0)
        return;

    textureContext = index;
    m_textureView->setTexture(m_textureList[textureContext]);
    activateTexture();
    emit updateGL();
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

void SLTextures::applyTextures(QGLShaderProgram* program)
{
    for(int i = 0; i < m_textureList.size(); ++i)
    {
        const char * n = m_textureList[i].varName().toAscii();
        program->setUniformValue(n, (GLint)i );
    }
}

void SLTextures::activateTexture(void)
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
        if(m_textureList[0].glTextureName() >= 0)
        {
            m_textureList.push_back(Texture());
            textureContext = m_textureList.size() - 1;
        }
        textureFileName(list[i]);
    }

    viewUpdateList();
    activateTexture();
    emit updateGL();
}
