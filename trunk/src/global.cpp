#include "global.h"

QString ShaderLab::shaderToStr(Shader s)
{
    switch(s)
    {
        case Vertex: return "Vertex";
        case Fragment: return "Fragment";
    }
}
