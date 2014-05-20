#include "slTexture2.h"
#include "Texture.h"

#include <QImage>
#include <QDebug>

#include "slShaderProgram2.h"
#include <QGLWidget>

SLTextures2::SLTextures2() :
    QObject(NULL)
{
}

SLTextures2::~SLTextures2()
{
}

const Texture& SLTextures2::at(int i)const
{
    return m_textureList[i];
}
Texture& SLTextures2::at(int i)
{
    return m_textureList[i];
}

int SLTextures2::size()const
{
    return m_textureList.size();
}

int SLTextures2::setTexture(const QString& imageFileName, int idx)
{
    ShaderLab *sl = ShaderLab::instance();

    if(!Texture::isValid(imageFileName))
        return -1;

    if( idx < 0 )
    {
        Texture t;
         m_textureList.append(t);
        idx = m_textureList.size() - 1;
    }else
    {
        clearTexture(idx);
    }

    QImage img(imageFileName);

    Texture t;
    t.setImage(img);
    t.setGLTextureName(sl->glContext()->bindTexture(imageFileName, GL_TEXTURE_2D));
    t.setVarName(SAMPLEPREFIX2D + QString::number(idx));
    t.setFileName(imageFileName);

    m_textureList[idx] = t;

    return idx;
}

void SLTextures2::clearTexture(int idx)
{
    ShaderLab *sl = ShaderLab::instance();

    sl->glContext()->deleteTexture(m_textureList[idx].textureId());
    m_textureList[idx].clean();
}

void SLTextures2::remakeVarNames()
{
    for(int i = 0; i < m_textureList.size(); i++)
    {
        m_textureList[i].setVarName(SAMPLEPREFIX2D + QString::number(i));
    }
}

void SLTextures2::removeTexture(int idx)
{
    clearTexture(idx);
    m_textureList.removeAt(idx);
    remakeVarNames();
}

QList<QPair<QIcon, QString> > SLTextures2::viewUpdateList(void)
{
    QList<QPair<QIcon, QString> > list;
    for(int i = 0; i < m_textureList.size(); ++i)
    {
        QPair<QIcon, QString> pair;
        pair.first = QIcon(QPixmap::fromImage(m_textureList[i].image()));
        pair.second = m_textureList[i].varName();
        list.push_back(pair);
    }

    return list;
}

void SLTextures2::setUniforms()
{
    for(int i = 0; i < m_textureList.size(); ++i)
    {
        if(m_textureList[i].uniformLocation() >= 0)
            glUniform1i(m_textureList[i].uniformLocation(), i);
    }
}

void SLTextures2::bind()
{
    for(int i = 0; i < m_textureList.size(); ++i)
    {
        if(m_textureList[i].uniformLocation() >= 0)
            glUniform1i(m_textureList[i].uniformLocation(), i);
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, m_textureList[i].textureId());
    }
    glActiveTexture(GL_TEXTURE0);
}

void SLTextures2::release(void)
{
    for(int i = 0; i < m_textureList.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, 0);
    }
    glActiveTexture(GL_TEXTURE0);
}

QStringList SLTextures2::getTextureFileNames()
{
    QStringList filenames;

    for(int i = 0; i < m_textureList.size(); ++i)
    {
        QString filename = m_textureList[i].filePath();
        if(!filename.isEmpty())
        {
            filenames.append(filename);
        }
    }

    return filenames;
}

void SLTextures2::removeAllTextures(void)
{
    while(m_textureList.size() > 0)
        removeTexture(0);
}

void SLTextures2::setTextures(const QStringList& list)
{
    removeAllTextures();

    for(int i = 0; i < list.size(); ++i)
    {
        setTexture(list[i]);
    }
}
