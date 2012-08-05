#ifndef SLOBJECTCONTROLLER_H
#define SLOBJECTCONTROLLER_H

#include <QObject>

class MainWindow;
class SLObject2;
class TexturePropertiesView;
class SLTexture3DDialog;
class PrimitivesDialog;
class Object3D;
class SLShaderProgramController;

class SLObjectController : public QObject
{
    Q_OBJECT

    MainWindow* mainWindow;
    SLObject2* object;
    TexturePropertiesView * m_texturePropertiesView;
    SLTexture3DDialog* m_texture3dDialog;
    PrimitivesDialog* m_primitivesDialog;
    SLShaderProgramController* m_programController;

public:
    explicit SLObjectController(MainWindow* mw, QObject *parent = 0);
    ~SLObjectController();

    SLObject2 * newObject(Object3D*);
    SLShaderProgramController* programController();

    void setTexturesFromProject(const QStringList& list);
    bool closeAllShaders();
    QString saveProject(bool as);
    void closeObject();

signals:

public slots:

};

#endif // SLOBJECTCONTROLLER_H
