#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <QObject>
#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>
#include <QVector>

class Object3D : public QObject
{
    Q_OBJECT

    QVector3D m_center;
    QVector3D m_translation;
    QMatrix4x4 m_rotacoes;
    QQuaternion m_interactiveQuartenion;

    //for glu objects
    int m_slices;
    int m_stacks;

    int m_texture;

    virtual void drawGeometry(void) const = 0;

public:
    explicit Object3D(const QVector3D& center = QVector3D(0.0,0.0,0.0),
                      QObject *parent = 0);
    explicit Object3D(const Object3D& obj);


    int slices(void)const;
    int stacks(void)const;

    //Esta em portugues
    void setRotacoes(const QMatrix4x4&);
    const QMatrix4x4& rotacoes(void)const;
    void addRotacao(const QQuaternion&);

    void setInteractiveQuartenion(const QQuaternion& quaternion);
    const QQuaternion& interactiveQuartenion(void)const;

    void setCenter(const QVector3D& center);
    const QVector3D& center(void)const;

    void setTraslation(const QVector3D& translation);
    const QVector3D& translation(void)const;

    void setTexture(int texture);
    int texture(void)const;

    void draw(void) const;

    void cleanTransformations();

protected:

    void setStacks(int stacks);
    void setSlices(int slices);

};

#endif // OBJECT3D_H
