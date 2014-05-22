#include "gl3w.h"
#include <QGLContext>
#include "Global.h"
#include <cmath>
#include <QDebug>

#ifndef GL_GEOMETRY_SHADER_EXT
#define GL_GEOMETRY_SHADER_EXT                      0x8DD9
#endif

#define TIMEINC 0.0005

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

int ShaderLab::shaderToGLShader(Shader s)
{
    switch(s)
    {
    case Vertex:
        return GL_VERTEX_SHADER;
    case Fragment:
        return GL_FRAGMENT_SHADER;
    case Geometry:
        if(geometryShaderOnlyEXT())
            return GL_GEOMETRY_SHADER_EXT;
        else
            return GL_GEOMETRY_SHADER;
    case TessellationCtrl:
        return GL_TESS_CONTROL_SHADER;
    case TessellationEval:
        return GL_TESS_EVALUATION_SHADER;
    }

    return -1;
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

unsigned int ShaderLab::shaderToIndex(Shader s)
{
    return static_cast<unsigned int>(s);
}

ShaderLab::Shader ShaderLab::indexToShader(unsigned int v)
{
    return static_cast<ShaderLab::Shader>(v);
}

void ShaderLab::extensionsAnalise()
{
    m_extensions = 0;

    QGLFormat::OpenGLVersionFlags oglvf =  QGLFormat::openGLVersionFlags ();

    qDebug() << oglvf;

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
        else if(ext == "GL_ARB_geometry_shader4" || ext == "GL_EXT_geometry_shader4")
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
    m_time = 0.0;
 }

void ShaderLab::incTime()
{
    m_time += TIMEINC;
    m_time = m_time >= 1.0 ? 0.0 : m_time;
}

float ShaderLab::time()const
{
    return m_time;
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

QString ShaderLab::toHtmlFormatParagraph(const QString& plainText)
{
    QString out;
    int ant = 0;

    for(int i = 0; i < plainText.size(); ++i)
    {
        if(plainText.at(i) == '\n')
        {
            QString tmp;
            for(int j = ant; j < i; ++j)
                tmp.append(plainText[j]);

            out += "<p>" + tmp + "</p>";
            ant = i + 1;
        }
    }
    return out;
}
