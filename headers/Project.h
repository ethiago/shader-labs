#ifndef PROJECT_H
#define PROJECT_H

#include <QDomNode>
#include <QObject>
#include <QDir>
#include <QMap>

#include "Global.h"
#include "FileController.h"

class Project : public QObject
{
    Q_OBJECT

    QMap<ShaderLab::Shader, QString> shaderFiles;
    typedef QMap<ShaderLab::Shader, QString>::iterator ShaderIterator;

    QFileInfo m_fileName;

public:
    explicit Project(QObject *parent = 0);

    bool load(const QString& fileName);

    bool includeShader(const FileController& fileController);

    bool save(QString fileName = QString());

    QString getFileName(ShaderLab::Shader);
    QString getRelativeFileName(ShaderLab::Shader);

    QDir getProjectDir(void);
    QString getProjectFileName(void);

    void removeShader(ShaderLab::Shader);

private:
    bool loadFileTag(QDomElement);

    QString getXml(void);

};

#endif // PROJECT_H
