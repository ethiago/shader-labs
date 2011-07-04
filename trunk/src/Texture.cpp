#include "Texture.h"

Texture::Texture(QObject *parent) :
    QObject(parent), m_image(QImage(":/ico/noImage")), m_glTextureName(-1)
{

}

void Texture::setImage(const QImage& image)
{
    m_image = image;
}

const QImage& Texture::image(void) const
{
    return m_image;
}

int Texture::glTextureName(void) const
{
    return m_glTextureName;
}

void Texture::setGLTextureName(int texName)
{
    m_glTextureName = texName;
}

void Texture::clean(void)
{
    m_image = QImage(":/ico/noImage");
    m_glTextureName = -1;
}
