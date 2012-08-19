#ifndef TEXTURE3D_H
#define TEXTURE3D_H

//#if defined(__WIN32__)
//    #include "GLee.h"
//#endif

#include "Texture.h"
#include "gl3w.h"

class Texture3D : public Texture
{
    GLubyte ***m_data;
    int m_width;
    int m_height;
    int m_depth;
public:
    Texture3D(QObject *parent = 0);
    ~Texture3D();

    void loadFromRawFile(const QString& fn, int width, int height, int depth, int len);

    void openGLBind();

    static GLuint openGLBind(GLubyte ***data, int width, int height, int detph);

};

#endif // TEXTURE3D_H
