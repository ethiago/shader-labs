#ifndef GLOBAL_H
#define GLOBAL_H

#include <QGLShader>
#include <QString>
#include <cmath>

#define QT47_CAPABLE

class ShaderLab {

    int m_extensions;

    static ShaderLab *s_instance;
    ShaderLab();

public:

    static ShaderLab *instance()
    {
        if (!s_instance)
            s_instance = new ShaderLab;
        return s_instance;
    }

    int extensions();

    enum Shader{ Vertex, Fragment, Geometry };

    enum ExtensionSupport{ FramebufferObject = 1,
                       VertexShader = 2,
                       GeometryShader = 4,
                       FragmentShader = 8,
                       ShaderObjects = 16};

    static QString shaderToStrCap(Shader s);
    static QString shaderToStr(Shader s);
    static QString shaderToExt(Shader s);

    static QGLShader::ShaderTypeBit shaderToQGLShader(Shader s);

    static int shaderToInt(Shader s);
    static Shader intToShader(int v);

    static double degreeFromCos(double);

    static QList<ShaderLab::Shader> shaderTypeList(void);
};

#define FORSHADERS(varname) foreach(ShaderLab::Shader varname, ShaderLab::shaderTypeList())


#endif // GLOBAL_H
