#include "Project.h"
#include "InterfaceRequests.h"
#include "Global.h"

#include <QFile>
#include <QDomDocument>
#include <QDomNode>
#include <QDebug>

Project::Project(QObject *parent) :
    QObject(parent), modelId(-1)
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

    loadModelTag(root);

    loadTextureTag(root);

    return loadFileTag(root);
}

bool Project::loadModelTag(QDomElement root)
{
    QDomElement child;
    QString value;

    root = root.firstChildElement("model");
    if(root.isNull())
        return false;

    //******ID******//
    child = root.firstChildElement("id");
    if(!child.isNull())
    {
        value = child.text();
        if(!value.isEmpty())
        {
            bool ret;
            int t = value.toInt(&ret);
            if(ret)
                modelId = t;

        }
    }
    //**************//

    return true;
}

bool Project::loadTextureTag(QDomElement root)
{
    QDomElement child;
    QString value;

    root = root.firstChildElement("textures");
    if(root.isNull())
        return false;

    textures.clear();
    //******filename******//
    child = root.firstChildElement("filename");
    while(!child.isNull())
    {
        value = child.text();
        if(!value.isEmpty())
            textures.append(value);

        child = child.nextSiblingElement("filename");
    }
    //**************//

    return true;
}

bool Project::loadFileTag(QDomElement root)
{
    QDomElement child;
    QString value;

    root = root.firstChildElement("shaders");
    if(root.isNull())
        return false;


    FORENABLEDSHADERS(shaderType)
    {
        child = root.firstChildElement(ShaderLab::shaderToStr(shaderType));
        if(!child.isNull())
        {
            value = child.text();
            if(!value.isEmpty())
                shaderFiles[shaderType] = value;
        }
    }


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

QString Project::getRelativeFilePathByShader(ShaderLab::Shader shadertype)
{
    ShaderIterator it = shaderFiles.find(shadertype);

    if(it == shaderFiles.end())
        return QString();
    else
    {
        return getRelativeFilePath(it.value());
    }
}

QString Project::getRelativeFilePath(QString filePath)
{
    QFileInfo fi(filePath);

    QDir d = m_fileName.absoluteDir();

    QString ret = d.relativeFilePath(fi.absoluteFilePath());

    return ret;
}

QDir Project::getProjectDir(void)
{
    return m_fileName.absoluteDir();
}

bool Project::includeShader(const SLFile& fileController)
{
    return includeShader(fileController.getFilePath(), fileController.shaderType());
}

bool Project::includeShader(const QString& filePath, ShaderLab::Shader shaderType)
{
    shaderFiles[shaderType] = filePath;

    return true;
}

void Project::checkShader(const QString& filePath, ShaderLab::Shader shaderType)
{
    ShaderIterator it = shaderFiles.find(shaderType);
    if(it == shaderFiles.end() && filePath.isEmpty())
        return;

    if(filePath.isEmpty())
    {
        //does remove?
        if(InterfaceRequests::removeFileFromProject(it.value()))
            removeShader(shaderType);
        return;
    }

    if(it == shaderFiles.end())
    {
        //does include?
        if(InterfaceRequests::includeFileIntoProject(filePath))
            includeShader(filePath, shaderType);
        return;
    }

    if(getFileName(shaderType) != filePath)
    {
        if(InterfaceRequests::replaceFileIntoProject(filePath))
            includeShader(filePath, shaderType);
        return;
    }
}

bool Project::save(QString fileName)
{
    if(fileName.isEmpty())
        fileName = m_fileName.absoluteFilePath();

    QFile file(fileName);

    if(file.open(QIODevice::WriteOnly | QIODevice::Text))
    {
        m_fileName = QFileInfo(fileName);

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

    for(it = shaderFiles.begin(); it != shaderFiles.end(); ++it)
    {

        QString otag = "\t\t<" + ShaderLab::shaderToStr(it.key()) + ">";
        QString ctag = "</" + ShaderLab::shaderToStr(it.key()) + ">\n";

        content += otag + getRelativeFilePath(it.value()) + ctag;
    }

    content = content +  "\t</shaders>\n";

    if(modelId >= 0)
    {
        content = content + "\t<model>\n"+
                "\t\t<id>" + QString::number(modelId) + "</id>\n\t</model>\n";
    }

    if(textures.size() > 0)
    {
        content = content + "\t<textures>\n";

        for(int i = 0; i < textures.size(); ++i)
        {
            content = content + "\t\t<filename>" + getRelativeFilePath(textures[i]) + "</filename>\n";
        }

        content = content + "\t</textures>\n";
    }

    content = content + "</ShaderLab>\n";

    return content;
}

void Project::removeShader(ShaderLab::Shader shaderType)
{
    shaderFiles.remove(shaderType);
}

QString Project::getProjectFileName(void)
{
    QString ret = m_fileName.fileName();

    if(ret.right(4) == ".slp")
        ret = ret.left(ret.length()-4);

    return ret;
}

QString Project::getAbsoluteFilePath(void)
{
    return m_fileName.absoluteFilePath();
}

void Project::setModel(int ind)
{
    modelId = ind;
}

int Project::getModelId(void)
{
    return modelId;
}

void Project::setTextures(const QStringList& texturesNames)
{
    textures.clear();

    for(int i = 0; i < texturesNames.size(); ++i)
    {
        QFileInfo fi(texturesNames[i]);

        QDir d = m_fileName.absoluteDir();

        textures.append(d.relativeFilePath(fi.filePath()));
    }
}

QStringList Project::getTextures(void) const
{
    QStringList ret;

    for(int i = 0; i < textures.size(); ++i)
    {
        QFileInfo fi(textures[i]);

        if(fi.isAbsolute())
        {
            ret.append(fi.absoluteFilePath());
        }
        else
        {
            QDir d = m_fileName.absoluteDir();
            ret.append(d.absoluteFilePath(fi.filePath()));
        }

    }

    return ret;
}
