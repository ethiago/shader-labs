#ifndef SLTEXTURECONTROLLER_H
#define SLTEXTURECONTROLLER_H

#include <QObject>
#include "gl3w.h"

class TexturePropertiesView;
class MainWindow;
class SLTextures2;
class Project;

class SLTextureController : public QObject
{
    Q_OBJECT

    TexturePropertiesView* m_textureView;

    SLTextures2 * e_textures;

public:
    explicit SLTextureController(MainWindow *mw);
    ~SLTextureController();

    void setTextures(SLTextures2 * textures);

private:
    void updateView();

public slots:
    void afterLink(GLuint programId);

    void changeTexture();
    void removeTexture();
    void addTexture();
    void textureCurrentChange(int);
    //
    void projectOpened(Project*);


};

#endif // SLTEXTURECONTROLLER_H
