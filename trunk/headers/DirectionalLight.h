#ifndef DIRECTIONALLIGHT_H
#define DIRECTIONALLIGHT_H

#include <QObject>
#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>
#include <QVector>

class DirectionalLight : public QObject
{

    QMatrix4x4 m_rotations;
    QQuaternion m_interactiveQuartenion;

    bool showLine;

public:
    explicit DirectionalLight(QObject *parent = 0);

    void setRotations(const QMatrix4x4&);
    const QMatrix4x4& rotations(void)const;
    void addRotation(const QQuaternion&);

    void setInteractiveQuartenion(const QQuaternion& quaternion);
    const QQuaternion& interactiveQuartenion(void)const;

    QVector3D getLightPosition()const;

    void setShowLine(bool);

    void draw(void) const;

    void cleanTransformations();

};

#endif // DIRECTIONALLIGHT_H
