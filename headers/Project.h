#ifndef PROJECT_H
#define PROJECT_H

#include <QDomNode>
#include <QObject>
#include <QDir>
#include <QMap>

#include "Global.h"

class Project : public QObject
{

    QMap<ShaderLab::Shader, QString> shaderFiles;
    typedef QMap<ShaderLab::Shader, QString>::iterator ShaderIterator;
    int modelId;
    QStringList textures;

    QFileInfo m_fileName;

    QString modelFileName;

    bool m_opened;

public:

    enum FileStatus{
        OK,
        IsDifferent,
        NotHave,
        Have
    };

    explicit Project();

    bool isOpened()const;

    bool load(const QString& fileName);
    void close();

    bool includeShader(const QString& filePath, ShaderLab::Shader shaderType);
    FileStatus checkShader(const QString& filePath, ShaderLab::Shader shaderType);

    bool save(QString fileName = QString());

    QString getFileName(ShaderLab::Shader);
    QString getRelativeFilePathByShader(ShaderLab::Shader);

    QDir getProjectDir(void);
    QString getProjectFileName(void);
    QString getAbsoluteFilePath(void);

    void removeShader(ShaderLab::Shader);

    void setModel(int);
    int getModelId(void);

    void setModelFileName(const QString& filename);
    QString getModelFileName(void);

    void setTextures(const QStringList&);
    QStringList getTextures(void) const;

    QString getRelativeFilePath(QString filePath);

private:
    bool loadFileTag(QDomElement);
    bool loadModelTag(QDomElement);
    bool loadTextureTag(QDomElement);

    QString resolvePath(QString relativePath);

    QString getXml(void);

};

#endif // PROJECT_H
