#ifndef SLOBJECT_H
#define SLOBJECT_H

#include <QObject>
#include <QColor>
#include "plydataitens.h"

class SLShaderProgram2;
class Object3D;
class SLTextures2;
class SLShaderCodes;
class Project;

class SLObject : public QObject
{
    Q_OBJECT

    SLShaderProgram2 * m_program;
    Object3D *m_object;
    SLTextures2 *m_textures;
    SLShaderCodes *m_shaderCodes;
    Project *m_project;

    bool m_visible;

    QColor m_color;

    bool m_wireframe;

public:
    explicit SLObject(Object3D *obj);
    virtual ~SLObject();

    void changeObject(Object3D *obj);
    void toggleWireframe();
    void draw();

    bool isVisible()const;
    void setVisible(bool v);

    int modelId()const;

    void setColor(const QColor& color);
    const QColor& color()const;

    SLShaderProgram2 * shaderProgram();
    SLShaderCodes * shaderCodes();
    SLTextures2* textures();
    Project *project();



    QList<PLYDataHeader::Property> getAttributeInfos()const;

};

#endif // SLOBJECT_H
