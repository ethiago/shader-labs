#include "TextureController.h"
#include "TexturePropertiesView.h"
#include "Texture.h"
#include "mainwindow.h"
#include <QImage>
#include <QFileDialog>

TextureController::TextureController(MainWindow* mw, QGLWidget* context,QObject *parent) :
    QObject(parent), m_texture(NULL), m_context(context)
{
   m_texture = new Texture(this);
   m_textureView = new TexturePropertiesView(mw);

   m_textureView->show();


   connect(mw, SIGNAL(textureFileName(QString)),
           this, SLOT(textureFileName(QString)));

   connect(mw, SIGNAL(removeTexture()),
           this, SLOT(removeTexture()));

   connect(m_textureView, SIGNAL(loadTextureClicked()),
           this, SLOT(loadTexture()));

   connect(m_textureView, SIGNAL(removeTextureClicked()),
           this, SLOT(removeTexture()));
}

void TextureController::loadTexture(void)
{
    QString filename = QFileDialog::getOpenFileName(m_textureView, "Open Image", ".");

    if(!filename.isEmpty())
        textureFileName(filename);
}

void TextureController::textureFileName(const QString& imageFileName)
{
    QImage img(imageFileName);
    if(img.isNull())
    {
        qDebug() << "Nao eh uma imagem valida";
        return;
    }

    removeTexture();

    m_texture->setImage(img);
    m_texture->setGLTextureName(m_context->bindTexture(imageFileName, GL_TEXTURE_2D));
    emit updateTexture(m_texture->glTextureName());
    m_textureView->setTexture(*m_texture);
    m_context->updateGL();
}

void TextureController::removeTexture(void)
{
    if(m_texture->glTextureName() >= 0)
    {
        m_context->deleteTexture(m_texture->glTextureName());
        m_texture->setGLTextureName(-1);
    }
    emit updateTexture(m_texture->glTextureName());
    m_textureView->setTexture(*m_texture);
    m_context->updateGL();
}
