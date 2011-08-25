#ifndef PROJECT_H
#define PROJECT_H

#include <QDomNode>
#include <QObject>
#include <QDir>
#include <QMap>

#include "Global.h"

class Project : public QObject
{
    Q_OBJECT

    QMap<ShaderLab::Shader, QString> shaderFiles;
    typedef QMap<ShaderLab::Shader, QString>::iterator ShaderIterator;

public:
    explicit Project(QObject *parent = 0);

    bool load(const QString& fileName);

    QString getFileName(ShaderLab::Shader);

private:
    bool loadFileTag(QDomElement);

};

#endif // PROJECT_H
