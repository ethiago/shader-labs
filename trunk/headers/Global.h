#ifndef GLOBAL_H
#define GLOBAL_H

#include <QGLShader>
#include <QString>

#define QT47_CAPABLE

namespace ShaderLab {

enum Shader{ Vertex, Fragment, Geometry };

QString shaderToStrCap(Shader s);
QString shaderToStr(Shader s);
QString shaderToExt(Shader s);

QGLShader::ShaderTypeBit shaderToQGLShader(Shader s);

int shaderToInt(Shader s);
Shader intToShader(int v);

double degreeFromCos(double);

enum OperationState{ Yes, No, Cancel};

}

#endif // GLOBAL_H
