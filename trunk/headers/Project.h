#ifndef PROJECT_H
#define PROJECT_H

#include <QDomNode>
#include <QObject>
#include <QDir>
#include <QMap>

#include "Global.h"
#include "SLFile.h"

class Project : public QObject
{
    Q_OBJECT

    QMap<ShaderLab::Shader, QString> shaderFiles;
    typedef QMap<ShaderLab::Shader, QString>::iterator ShaderIterator;
    int modelId;
    QStringList textures;

    QFileInfo m_fileName;

public:
    explicit Project(QObject *parent = 0);

    bool load(const QString& fileName);

    bool includeShader(const SLFile& fileController);

    bool save(QString fileName = QString());

    QString getFileName(ShaderLab::Shader);
    QString getRelativeFileName(ShaderLab::Shader);

    QDir getProjectDir(void);
    QString getProjectFileName(void);

    void removeShader(ShaderLab::Shader);

    void setModel(int);
    int getModelId(void);

    void setTextures(const QStringList&);
    QStringList getTextures(void) const;

private:
    bool loadFileTag(QDomElement);
    bool loadModelTag(QDomElement);
    bool loadTextureTag(QDomElement);

    QString getXml(void);

};

#endif // PROJECT_H
