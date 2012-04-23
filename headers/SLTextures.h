#ifndef SLTEXTURES_H
#define SLTEXTURES_H

#if defined(__WIN32__)
#include <GLee.h>
#endif
#include <QObject>
#include <QGLContext>
#include <QList>
#include <QGLShaderProgram>
#include "Texture.h"

#define SAMPLEPREFIX2D "sampler2d"
#define SAMPLEPREFIX3D "sampler3d"

class MainWindow;
class TexturePropertiesView;
class SLTexture3DDialog;

class SLTextures : public QObject
{
    Q_OBJECT

    QList<Texture> m_textureList;
    int textureContext;
    TexturePropertiesView* m_textureView;
    SLTexture3DDialog *m_texture3DDialog;

signals:
    void updateGL();

public:
    explicit SLTextures(MainWindow* mw, QObject* parent = 0);
    virtual ~SLTextures();

    void applyTextures(QGLShaderProgram* program);
    void activateTexture(void);
    QStringList getTextureFileNames();
    void setTextures(const QStringList&);
    void closeView(MainWindow *mw);

public slots:
    void removeTexture(void);
    void changeTexture(void);
    void addTexture(void);
    void textureCurrentChange(int);
    void addTexture3D(void);

private:
    void remakeVarNames();
    void setupTexture(const QString& imageFileName, bool add);
    void viewUpdateList(void);
    void clearTexture(void);
    void removeAllTextures(void);

};

#endif // SLTEXTURES_H
