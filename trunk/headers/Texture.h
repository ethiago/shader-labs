#ifndef TEXTURE_H
#define TEXTURE_H

#include <QObject>
#include <QImage>

class Texture : public QObject
{
    QImage m_image;
    int m_glTextureName;
    QString m_varName;
    QString m_filename;
    bool m_2d;

public:
    static bool isValid(const QString& filePath);

    explicit Texture(QObject *parent = 0);
    Texture(const Texture&);

    void setImage(const QImage& image);
    const QImage& image(void) const;

    int glTextureName(void)const;
    void setGLTextureName(int);

    const QString& varName(void) const;
    void setVarName(const QString&);

    const QString& fullFileName(void) const;
    QString fileName(void) const;
    void setFileName(const QString& fn);

    void clean(void);

    bool is2D()const;

    const Texture& operator=(const Texture&);

protected:
    void set3D();

};

#endif // TEXTURE_H
