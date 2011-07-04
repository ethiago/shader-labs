#ifndef TEXTURECONTROLLER_H
#define TEXTURECONTROLLER_H

#include <QObject>
#include <QGLContext>

class MainWindow;
class Texture;
class TexturePropertiesView;

class TextureController : public QObject
{
    Q_OBJECT

    Texture* m_texture;
    QGLWidget* m_context;
    TexturePropertiesView* m_textureView;

public:
    explicit TextureController(MainWindow* mw, QGLWidget*, QObject *parent = 0);

signals:
    void updateTexture(int);

public slots:
    void textureFileName(const QString&);
    void removeTexture(void);
    void loadTexture(void);


};

#endif // TEXTURECONTROLLER_H
