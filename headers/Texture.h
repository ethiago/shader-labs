#ifndef TEXTURE_H
#define TEXTURE_H

#include <QObject>
#include <QImage>
#include <QIcon>

class Texture : public QObject
{
    QImage m_image;
    int m_glTextureName;
    QString m_varName;
    QString m_filename;
    int m_uniformLocation;

public:
    static bool isValid(const QString& filePath);

    explicit Texture();
    Texture(const Texture&);

    void setImage(const QImage& image);
    const QImage& image(void) const;
    QIcon icon(void)const;

    int textureId(void)const;
    void setGLTextureName(int);

    const QString& varName(void) const;
    void setVarName(const QString&);

    const QString& filePath(void) const;
    QString fileName(void) const;
    void setFileName(const QString& fn);

    void setUniformLocation(int id);
    int uniformLocation(void)const;

    void clean(void);

    const Texture& operator=(const Texture&);

};

#endif // TEXTURE_H
