#ifndef GLOBAL_H
#define GLOBAL_H

#include <QString>

class QGLWidget;

#define QT47_CAPABLE

class ShaderLab {

    int m_extensions;

    QGLWidget *m_context;

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

    enum Shader{ Vertex, Fragment, Geometry, TessellationEval, TessellationCtrl };

    enum ExtensionSupport{ FramebufferObject = 1,
                       VertexShader = 2,
                       GeometryShaderARB = 4,
                       GeometryShaderEXT = 8,
                       FragmentShader = 16,
                       TessellationShader = 32,
                       ShaderObjects = 64};

    void extensionsAnalise();
    bool criticalExtensionsEnabled();
    bool vertexShaderEnabled();
    bool fragmentShaderEnabled();
    bool geometryShaderEnabled();
    bool geometryShaderOnlyEXT();
    bool tesselationShaderEnable();
    void setContext(QGLWidget*);
    QGLWidget* glContext();


    static QString shaderToStrCap(Shader s);
    static QString shaderToStr(Shader s);
    static QString shaderToExt(Shader s);

    //tatic QGLShader::ShaderTypeBit shaderToQGLShader(Shader s);

    static int shaderToInt(Shader s);
    static Shader intToShader(int v);

    static QList<ShaderLab::Shader> shaderTypeList(void);
    static QList<ShaderLab::Shader> enabledsShaderTypeList(void);

};

#define FORSHADERS(varname) foreach(ShaderLab::Shader varname, ShaderLab::shaderTypeList())
#define FORENABLEDSHADERS(varname) foreach(ShaderLab::Shader varname, ShaderLab::enabledsShaderTypeList())

#endif // GLOBAL_H
