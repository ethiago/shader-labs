#ifndef TEXTURE_H
#define TEXTURE_H

#include <QObject>
#include <QImage>
#include <QGLContext>

class Texture : public QObject
{
    Q_OBJECT

    QImage m_image;
    int m_glTextureName;

public:
    explicit Texture(QObject *parent = 0);
    explicit Texture(const Texture&);

    void setImage(const QImage& image);
    const QImage& image(void) const;

    int glTextureName(void)const;
    void setGLTextureName(int);

    void clean(void);

    const Texture& operator=(const Texture&);

signals:

public slots:

};

#endif // TEXTURE_H
