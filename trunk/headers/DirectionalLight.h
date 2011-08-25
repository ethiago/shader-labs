#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include <QObject>
#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>
#include <QVector>

class DirectionalLight : public QObject
{
    Q_OBJECT

    QMatrix4x4 m_rotations;
    QQuaternion m_interactiveQuartenion;

public:
    explicit DirectionalLight(QObject *parent = 0);

    void setRotations(const QMatrix4x4&);
    const QMatrix4x4& rotations(void)const;
    void addRotation(const QQuaternion&);

    void setInteractiveQuartenion(const QQuaternion& quaternion);
    const QQuaternion& interactiveQuartenion(void)const;

    void draw(void) const;

    void cleanTransformations();

};

#endif // DIRECTIONALLIGHT_H
