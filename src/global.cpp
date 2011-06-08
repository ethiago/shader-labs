#include "global.h"
#include "cmath"

QString ShaderLab::shaderToStr(Shader s)
{
    switch(s)
    {
        case Vertex: return "vertex";
        case Fragment: return "fragment";
        case Geometry: return "geometry";
    }

    return QString();
}

QString ShaderLab::shaderToStrCap(Shader s)
{
    switch(s)
    {
        case Vertex: return "Vertex";
        case Fragment: return "Fragment";
        case Geometry: return "Geometry";
    }

    return QString();
}

QString ShaderLab::shaderToExt(Shader s)
{
    switch(s)
    {
        case Vertex: return ".vert";
        case Fragment: return ".frag";
        case Geometry: return ".geom";
    }

    return QString();

}

QGLShader::ShaderTypeBit ShaderLab::shaderToQGLShader(Shader s)
{
    switch(s)
    {
        case Vertex: return QGLShader::Vertex;
        case Fragment: return QGLShader::Fragment;
#ifdef QT47_CAPABLE
        case Geometry: return QGLShader::Geometry;
#endif
    }

    return QGLShader::Vertex;
}

int ShaderLab::shaderToInt(Shader s)
{
    return static_cast<int>(s);
}

ShaderLab::Shader ShaderLab::intToShader(int v)
{
    return static_cast<ShaderLab::Shader>(v);
}

double ShaderLab::degreeFromCos(double _cos)
{
    return (acos(_cos)*180.0)/M_PI;
}
