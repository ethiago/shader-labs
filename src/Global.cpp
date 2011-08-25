#include "Global.h"

ShaderLab* ShaderLab::s_instance = 0;

QString ShaderLab::shaderToStr(Shader s)
{
    switch(s)
    {
        case Vertex: return "vertex";
        case Fragment: return "fragment";
        case Geometry: return "geometry";
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
    }

    return QString();

}

QGLShader::ShaderTypeBit ShaderLab::shaderToQGLShader(Shader s)
{
    switch(s)
    {
        case Vertex: return QGLShader::Vertex;
        case Fragment: return QGLShader::Fragment;
#ifdef QT47_CAPABLE
        case Geometry: return QGLShader::Geometry;
#endif
    }

    return QGLShader::Vertex;
}

int ShaderLab::shaderToInt(Shader s)
{
    return static_cast<int>(s);
}

ShaderLab::Shader ShaderLab::intToShader(int v)
{
    return static_cast<ShaderLab::Shader>(v);
}

double ShaderLab::degreeFromCos(double _cos)
{
    return (acos(_cos)*180.0)/M_PI;
}

ShaderLab::ShaderLab()
 {
    m_extensions = 0;

    const char *extensionString = reinterpret_cast<const char *>(glGetString(GL_EXTENSIONS));

    if(extensionString == 0)
       return;

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
            m_extensions |= ShaderLab::GeometryShader;
        else if(ext == "GL_ARB_shader_objects")
            m_extensions |= ShaderLab::ShaderObjects;
    }
 }

int ShaderLab::extensions()
{
    return m_extensions;
}
