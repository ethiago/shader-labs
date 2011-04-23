#ifndef GLOBAL_H
#define GLOBAL_H

#include <QGLShader>
#include <QString>

namespace ShaderLab {

enum Shader{ Vertex, Fragment };

QString shaderToStrCap(Shader s);
QString shaderToStr(Shader s);
QString shaderToExt(Shader s);

QGLShader::ShaderTypeBit shaderToQGLShader(Shader s);

int shaderToInt(Shader s);
Shader intToShader(int v);

}

#endif // GLOBAL_H
