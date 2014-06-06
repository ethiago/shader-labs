#include "gl3w.h"
#include "slgl3w.h"

SLGl3W::SLGl3W()
{
}

void SLGl3W::setPatchVertices(int n)
{
    glPatchParameteri(GL_PATCH_VERTICES, n);
}

int SLGl3W::getUniformLocation(unsigned int program, const QString& name)
{
    GLint location = -1;
    int value = 0;
    const char * cname = name.toAscii();
    glGetProgramiv(program, GL_LINK_STATUS, &value);
    if(value != 0)
        location = glGetUniformLocation(program, cname);

    return location;
}

int SLGl3W::getAttributeLocation(unsigned int program, const QString& name)
{
    GLint location = -1;
    int value = 0;
    const char * cname = name.toAscii();
    glGetProgramiv(program, GL_LINK_STATUS, &value);
    if(value != 0)
        location = glGetAttribLocation(program, cname);

    return location;
}

void SLGl3W::vertexAttrib(int loc, float value, bool isInt)
{
    if(!isInt)
        glVertexAttrib1f(loc, value );
    else
        glVertexAttribI1i(loc, qRound(value) );
}

void SLGl3W::vertexAttrib(int loc, const QVector2D& v, bool isInt)
{
    if(!isInt)
        glVertexAttrib2f(loc, v.x(), v.y() );
    else
        glVertexAttribI2i(loc, qRound(v.x()), qRound(v.y()) );
}

void SLGl3W::vertexAttrib(int loc, const QVector3D& v, bool isInt)
{
    if(!isInt)
        glVertexAttrib3f(loc, v.x(), v.y(), v.z() );
    else
        glVertexAttribI3i(loc, qRound(v.x()), qRound(v.y()), qRound(v.z()) );
}

void SLGl3W::vertexAttrib(int loc, const QVector4D& v, bool isInt)
{
    if(!isInt)
        glVertexAttrib4f(loc, v.x(), v.y(), v.z(), v.w() );
    else
        glVertexAttribI4i(loc, qRound(v.x()), qRound(v.y()), qRound(v.z()), qRound(v.w()) );
}

void SLGl3W::uniform(int loc, float value, bool isInt)
{
    if(!isInt)
        glUniform1f(loc, value );
    else
        glUniform1i(loc, qRound(value) );
}

void SLGl3W::uniform(int loc, const QVector2D& v, bool isInt)
{
    if(!isInt)
        glUniform2f(loc, v.x(), v.y() );
    else
        glUniform2i(loc, qRound(v.x()), qRound(v.y()) );
}

void SLGl3W::uniform(int loc, const QVector3D& v, bool isInt)
{
    if(!isInt)
        glUniform3f(loc, v.x(), v.y(), v.z() );
    else
        glUniform3i(loc, qRound(v.x()), qRound(v.y()), qRound(v.z()) );
}

void SLGl3W::uniform(int loc, const QVector4D& v, bool isInt)
{
    if(!isInt)
        glUniform4f(loc, v.x(), v.y(), v.z(), v.w() );
    else
        glUniform4i(loc, qRound(v.x()), qRound(v.y()), qRound(v.z()), qRound(v.w()) );
}
