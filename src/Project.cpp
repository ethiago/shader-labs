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

    m_fileName = QFileInfo(fileName);

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
    {
        QFileInfo fi(it.value());

        if(fi.isAbsolute())
            return fi.absoluteFilePath();

        QDir d = m_fileName.absoluteDir();

        return d.absoluteFilePath(fi.filePath());
    }
}

QString Project::getRelativeFileName(ShaderLab::Shader shadertype)
{
    ShaderIterator it = shaderFiles.find(shadertype);

    if(it == shaderFiles.end())
        return QString();
    else
    {
        QFileInfo fi(it.value());

        if(fi.isAbsolute())
            return fi.absoluteFilePath();

        QDir d = m_fileName.absoluteDir();

        return d.relativeFilePath(fi.filePath());
    }
}

QDir Project::getProjectDir(void)
{
    return m_fileName.absoluteDir();
}

bool Project::includeShader(const FileController& fileController)
{
    shaderFiles[fileController.getShaderType()] = fileController.getFilePath();

    qDebug() << shaderFiles;

    return true;
}

bool Project::save(const QString& fileName)
{
    QFile file(fileName);

    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        QTextStream out(&file);
        out << getXml();
        file.close();

        return true;
    }
    else
        return false;
}

QString Project::getXml(void)
{
    QString content = QString("<?xml version='1.0' encoding='UTF-8'?>\n") +
            "<ShaderLab>\n" +
            "\t<shaders>\n";

    QMap<ShaderLab::Shader, QString>::iterator it;
    qDebug() << "Qtd: " << shaderFiles.size();

    for(it = shaderFiles.begin(); it != shaderFiles.end(); ++it)
    {
        qDebug() << "Ahhhhhh!";

        QString otag = "\t\t<" + ShaderLab::shaderToStr(it.key()) + ">";
        QString ctag = "</" + ShaderLab::shaderToStr(it.key()) + ">\n";

        content += otag + getRelativeFileName(it.key()) + ctag;
    }

    content = content +  "\t</shaders>\n" +
            "</ShaderLab>\n";

    return content;
}
