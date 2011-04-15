#include "global.h"

QString ShaderLab::shaderToStr(Shader s)
{
    switch(s)
    {
        case Vertex: return "Vertex";
        case Fragment: return "Fragment";
    }

    return QString();
}

int ShaderLab::shaderToInt(Shader s)
{
    return static_cast<int>(s);
}

ShaderLab::Shader ShaderLab::intToShader(int v)
{
    return static_cast<ShaderLab::Shader>(v);
}
