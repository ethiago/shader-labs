#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <QObject>
#include <QVector3D>
#include <QQuaternion>

class Object3D : public QObject
{
    Q_OBJECT

    QVector3D m_center;
    QVector3D m_translation;
    QQuaternion m_quaternion;

    //for glu objects
    int m_slices;
    int m_stacks;

public:
    explicit Object3D(const QVector3D& center = QVector3D(0.0,0.0,0.0),
                      QObject *parent = 0);
    explicit Object3D(const Object3D& obj);

    void setQuaternion(const QQuaternion& quaternion);
    const QQuaternion& quaternion(void)const;

    void setCenter(const QVector3D& center);
    const QVector3D& center(void)const;

    void setSlices(int slices);
    int slices(void)const;

    void setStacks(int stacks);
    int stacks(void)const;

    void setTraslation(const QVector3D& translation);
    const QVector3D& translation(void)const;

    virtual void draw(void)const = 0;

signals:

public slots:

};

#endif // OBJECT3D_H
