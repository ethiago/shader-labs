#ifndef SLTEXTURES2_H
#define SLTEXTURES2_H

#include <QObject>
#include <QList>
#include <QPair>
#include <QIcon>
#include "Texture.h"
#include "gl3w.h"

#define SAMPLEPREFIX2D "sltexture"

class SLTextures2 : public QObject
{
    Q_OBJECT

    QList<Texture> m_textureList;

signals:
    void updateGL();

public:
    explicit SLTextures2();
    virtual ~SLTextures2();

    void bind();
    void setUniforms();

    void release(void);

    //for project
    QStringList getTextureFileNames();
    void setTextures(const QStringList&);

    //for controller
    int setTexture(const QString& imageFileName, int idx=-1);
    void removeTexture(int);
    const Texture& at(int i)const;
    Texture& at(int i);
    int size()const;

    QList<QPair<QIcon, QString> > viewUpdateList(void);

private:
    void remakeVarNames();
    void removeAllTextures(void);
    void clearTexture(int idx);

};

#endif // SLTEXTURES2_H
