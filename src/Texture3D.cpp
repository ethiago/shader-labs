
#include "Texture3D.h"
#include <QFile>
#include <QDebug>

Texture3D::Texture3D(QObject *parent): Texture(parent)
{
    setImage(QImage(":/ico/tex3d"));
    set3D();
    m_data = NULL;
}

Texture3D::~Texture3D()
{
    if(m_data)
    {
        for(int i = 0; i < m_width; ++i)
        {
            for(int j = 0; j  < m_height; ++j)
                delete m_data[i][j];

            delete m_data[i];
        }
        delete m_data;
    }
}

void Texture3D::loadFromRawFile(const QString& fn, int width, int height, int depth, int len)
{
    QFile f(fn);
    if(!f.open(QIODevice::ReadOnly))
        return;
    QDataStream s(&f);
    GLubyte v1;
    GLushort v2;

    m_width = width;
    m_height = height;
    m_depth = depth;

    m_data = new GLubyte**[m_width];
    for(int i = 0; i < m_width; ++i)
    {
        m_data[i] = new GLubyte*[m_height];
        for(int j = 0; j < m_height; ++j)
        {
            m_data[i][j] = new GLubyte[m_depth];
            for(int k = 0; k < m_depth; ++k)
            {
                if(len == 1)
                {
                    s >> v1;
                    m_data[i][j][k] = v1;
                }
                else
                {
                    s >> v2;
                    m_data[i][j][k] = v2/2;
                }
            }
        }
    }
    setFileName(fn);
}

GLuint Texture3D::openGLBind(GLubyte ***data, int width, int height, int detph)
{
    GLuint texname;
    glPixelStorei(GL_UNPACK_ALIGNMENT,1);
    glGenTextures(1, &texname);
    glBindTexture(GL_TEXTURE_3D, texname);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_S, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_T, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_WRAP_R, GL_CLAMP);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MIN_FILTER, GL_NEAREST);
    glTexParameteri(GL_TEXTURE_3D, GL_TEXTURE_MAG_FILTER, GL_NEAREST);

    //glTexImage3D(GL_PROXY_TEXTURE_3D, 0, GL_LUMINANCE, width, height, detph, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);
    glTexImage3D(GL_TEXTURE_3D, 0, GL_LUMINANCE, 180, 180, 180, 0, GL_LUMINANCE, GL_UNSIGNED_BYTE, data);

    return texname;
}

void Texture3D::openGLBind()
{
    setGLTextureName(Texture3D::openGLBind(m_data, m_width, m_height, m_depth));
}
