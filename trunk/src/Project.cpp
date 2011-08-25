#include <QFile>
#include <QDomDocument>
#include <QDomNode>
#include <QDebug>

#include "Project.h"

Project::Project(QObject *parent) :
    QObject(parent)
{
}

bool Project::load(const QString& fileName)
{
    QString erroMsg;
    int erroL;
    int erroC;

    QFile f(fileName);

    if(!f.open(QIODevice::ReadOnly | QIODevice::Text))
        return false;

    QDomDocument document;

    if(!document.setContent(f.readAll(), &erroMsg, &erroL, &erroC))
    {
        qDebug() << erroL << ":" << erroC;
        qDebug() << erroMsg;
        f.close();
        return false;
    }
    f.close();

    QDomElement root = document.documentElement();
    if(root.tagName() != "ShaderLab")
        return false;

    if(root.hasAttribute("version") && root.attribute("version") != "1.0")
        return false;

    return loadFileTag(root);
}

bool Project::loadFileTag(QDomElement root)
{
    QDomElement child;
    QString value;

    root = root.firstChildElement("shaders");
    if(root.isNull())
        return false;

    //******Vertex******//
    child = root.firstChildElement("vertex");
    if(!child.isNull())
    {
        value = child.text();
        if(!value.isEmpty())
            shaderFiles[ShaderLab::Vertex] = value;
    }
    //******************//

    //******Fragment******//
    child = root.firstChildElement("fragment");
    if(!child.isNull())
    {
        value = child.text();
        if(!value.isEmpty())
            shaderFiles[ShaderLab::Fragment] = value;
    }
    //********************//

    //******Geometry******//
    child = root.firstChildElement("geometry");
    if(!child.isNull())
    {
        value = child.text();
        if(!value.isEmpty())
            shaderFiles[ShaderLab::Geometry] = value;
    }
    //********************//

    return true;
}

QString Project::getFileName(ShaderLab::Shader shadertype)
{
    ShaderIterator it = shaderFiles.find(shadertype);

    if(it == shaderFiles.end())
        return QString();
    else
        return it.value();
}
