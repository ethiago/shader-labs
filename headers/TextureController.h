#ifndef TEXTURECONTROLLER_H
#define TEXTURECONTROLLER_H

#if defined(__WIN32__)
    #include <GLee.h>
#endif
#include <QObject>
#include <QGLContext>
#include <QList>
#include <QGLShaderProgram>
#include "Texture.h"

#define SAMPLEPREFIX "sampler2d"

class MainWindow;
class TexturePropertiesView;

class TextureController : public QObject
{
    Q_OBJECT

    QList<Texture> m_textureList;
    int textureContext;
    QGLWidget* m_context;
    TexturePropertiesView* m_textureView;
    QAction* m_viewAction;


public:
    explicit TextureController(MainWindow* mw, QGLWidget*, QObject *parent = 0);

    void applyTextures(QGLShaderProgram* program);
    void activateTexture(void);


signals:
    void updateTexture(int);

public slots:
    void textureFileName(const QString&);
    void removeTexture(void);
    void loadTexture(void);
    void addTexture(void);
    void viewToogle(bool);
    void viewCloseEvent();
    void textureCurrentChange(int);

private:
    void viewUpdateList(void);
    void clearTexture(void);

};

#endif // TEXTURECONTROLLER_H