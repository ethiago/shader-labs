#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>

namespace ShaderLab {

enum Shader{ Vertex, Fragment };
QString shaderToStr(Shader s);
int shaderToInt(Shader s);
Shader intToShader(int v);

}

#endif // GLOBAL_H
