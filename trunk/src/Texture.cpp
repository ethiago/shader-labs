#include "Texture.h"
#include <QFileInfo>

Texture::Texture(QObject *parent) :
    QObject(parent), m_image(QImage(":/ico/noImage")),
    m_glTextureName(-1), m_varName(QString()), m_filename(QString()), m_2d(true)
{
}

Texture::Texture(const Texture& tex) :
    QObject(tex.parent()), m_image(tex.image()),
    m_glTextureName(tex.glTextureName()), m_varName(tex.varName()),
    m_filename(tex.fullFileName()), m_2d(tex.is2D())
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
    m_varName = QString();
    m_filename = QString();
}

const QString& Texture::varName(void) const
{
    return m_varName;
}

void Texture::setVarName(const QString& varName)
{
    m_varName = varName;
}

QString Texture::fileName(void) const
{
    QString var = m_filename;
    if(var.isEmpty())
        return QString();

    QFileInfo fi(var);
    return fi.fileName();
}

const QString& Texture::fullFileName(void) const
{
    return m_filename;
}

void Texture::setFileName(const QString& fn)
{
    m_filename = fn;
}

const Texture& Texture::operator=(const Texture& tex)
{
    setParent(tex.parent());
    m_image = tex.image();
    m_glTextureName = tex.glTextureName();
    m_varName = tex.varName();
    m_filename = tex.fullFileName();
    m_2d = tex.is2D();
    return *this;
}

bool Texture::isValid(const QString& filePath)
{
    QImage i(filePath);
    return !(i.isNull());
}

bool Texture::is2D()const
{
    return m_2d;
}

void Texture::set3D()
{
    m_2d = false;
}
