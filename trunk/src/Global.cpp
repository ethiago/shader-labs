#include "gl3w.h"
#include <QGLContext>
#include "Global.h"
#include <cmath>
#include <QDebug>

ShaderLab* ShaderLab::s_instance = 0;

QString ShaderLab::shaderToStr(Shader s)
{
    switch(s)
    {
        case Vertex: return "vertex";
        case Fragment: return "fragment";
        case Geometry: return "geometry";
        case TessellationCtrl: return "tessellation_control";
        case TessellationEval: return "tessellation_evaluation";
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
        case TessellationCtrl: return "TessellationControl";
        case TessellationEval: return "TessellationEvaluation";
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
        case TessellationCtrl: return ".tsct";
        case TessellationEval: return ".tsev";
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

void ShaderLab::extensionsAnalise()
{
    m_extensions = 0;
    const char *extensionString = reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS));
    const char *renderer = reinterpret_cast<const char *>(glGetString(GL_RENDERER));

    if(extensionString == 0)
       return;

    QString rend(renderer);
    qDebug() << rend;
    QStringList extension = QString( extensionString).split(QChar(' '));
    qDebug() << extension;

    foreach(QString ext , extension)
    {
        if(ext == "GL_EXT_framebuffer_object")
            m_extensions |= ShaderLab::FramebufferObject;
        else if(ext == "GL_ARB_vertex_shader")
            m_extensions |= ShaderLab::VertexShader;
        else if(ext == "GL_ARB_fragment_shader")
            m_extensions |= ShaderLab::FragmentShader;
        else if(ext == "GL_ARB_geometry_shader4")
            m_extensions |= ShaderLab::GeometryShaderARB;
        else if(ext == "GL_EXT_geometry_shader4")
            m_extensions |= ShaderLab::GeometryShaderEXT;
        else if(ext == "GL_ARB_tessellation_shader")
            m_extensions |= ShaderLab::TessellationShader;
        else if(ext == "GL_ARB_shader_objects")
            m_extensions |= ShaderLab::ShaderObjects;
    }   
}

ShaderLab::ShaderLab()
 {
    m_context = NULL;
 }

int ShaderLab::extensions()
{
    return m_extensions;
}

QList<ShaderLab::Shader> ShaderLab::shaderTypeList(void)
{
    QList<ShaderLab::Shader> list;
    list.append(Vertex);
    list.append(Geometry);
    list.append(Fragment);
    list.append(TessellationEval);
    list.append(TessellationCtrl);

    return list;
}

QList<ShaderLab::Shader> ShaderLab::enabledsShaderTypeList(void)
{
    ShaderLab *sl = ShaderLab::instance();
    QList<ShaderLab::Shader> list;
    if(sl->vertexShaderEnabled())
        list.append(Vertex);
    if(sl->geometryShaderEnabled())
        list.append(Geometry);
    if(sl->fragmentShaderEnabled())
        list.append(Fragment);
    if(sl->tesselationShaderEnable())
    {
        list.append(TessellationEval);
        list.append(TessellationCtrl);
    }

    return list;
}

bool ShaderLab::criticalExtensionsEnabled()
{
    return ((this->extensions() & ShaderLab::FramebufferObject)  != 0 &&
            (this->extensions() & ShaderLab::ShaderObjects) != 0);
}

bool ShaderLab::vertexShaderEnabled()
{
    return (this->extensions() & ShaderLab::VertexShader)  != 0;
}

bool ShaderLab::fragmentShaderEnabled()
{
    return (this->extensions() & ShaderLab::FragmentShader)  != 0;
}

bool ShaderLab::geometryShaderEnabled()
{
    return (this->extensions() & ShaderLab::GeometryShaderARB  ||
            this->extensions() & ShaderLab::GeometryShaderEXT) != 0;
}

bool ShaderLab::geometryShaderOnlyEXT()
{
    return (!(this->extensions() & ShaderLab::GeometryShaderARB) &&
              this->extensions() & ShaderLab::GeometryShaderEXT) != 0;
}

bool ShaderLab::tesselationShaderEnable()
{
    return (this->extensions() & ShaderLab::TessellationShader)  != 0;
}

void ShaderLab::setContext(QGLWidget* context)
{
    m_context = context;
    gl3wInit();
}

QGLWidget* ShaderLab::glContext()
{
    return m_context;
}
