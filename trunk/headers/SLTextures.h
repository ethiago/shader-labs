#ifndef SLTEXTURES_H
#define SLTEXTURES_H

#include <QObject>
#include <QList>
#include "Texture.h"

#define SAMPLEPREFIX2D "sampler2d"
#define SAMPLEPREFIX3D "sampler3d"

class MainWindow;
class TexturePropertiesView;
class SLShaderProgram;

class SLTextures : public QObject
{
    Q_OBJECT

    QList<Texture> m_textureList;
    int textureContext;
    TexturePropertiesView* m_textureView;

signals:
    void updateGL();

public:
    explicit SLTextures(TexturePropertiesView*, QObject* parent = 0);
    virtual ~SLTextures();

    void setUniformTexture(SLShaderProgram*);
    void setupUniformTextureNames(SLShaderProgram*);
    void activeTextures(void);
    QStringList getTextureFileNames();
    void setTextures(const QStringList&);


public slots:
    void removeTexture(void);
    void changeTexture(void);
    void addTexture(void);
    void textureCurrentChange(int);

private:
    void remakeVarNames();
    void setupTexture(const QString& imageFileName, bool add);
    void viewUpdateList(void);
    void clearTexture(void);
    void removeAllTextures(void);

};

#endif // SLTEXTURES_H
