#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>

namespace ShaderLab {

enum Shader{ Vertex, Fragment };
QString shaderToStr(Shader s);

}

#endif // GLOBAL_H
