#ifndef OBJECT3D_H
#define OBJECT3D_H

#include <QObject>
#include <QVector3D>
#include <QQuaternion>
#include <QMatrix4x4>
#include <QVector>
#include <QColor>
#include "plydataitens.h"

class Object3D : public QObject
{
    QVector3D m_center;
    QVector3D m_translation;
    QMatrix4x4 m_rotations;
    QQuaternion m_interactiveQuartenion;

    //for glu objects
    int m_slices;
    int m_stacks;

    int m_inputType;

    int m_modelId;

    virtual void drawGeometry(void) const = 0;

public:
    explicit Object3D(const QVector3D& center = QVector3D(0.0,0.0,0.0));
    explicit Object3D(const Object3D& obj);
    virtual ~Object3D();


    int slices(void)const;
    int stacks(void)const;

    void setRotations(const QMatrix4x4&);
    const QMatrix4x4& rotations(void)const;
    void addRotation(const QQuaternion&);

    void setInteractiveQuartenion(const QQuaternion& quaternion);
    const QQuaternion& interactiveQuartenion(void)const;

    void setCenter(const QVector3D& center);
    const QVector3D& center(void)const;

    void setTraslation(const QVector3D& translation);
    const QVector3D& translation(void)const;

    void setModelId(int id);
    int modelId(void) const;

    int inputType(void)const;

    void draw(const QColor& cor = QColor()) const;

    void cleanTransformations();

    virtual Object3D* copy(void) const = 0;

    static double degreeFromCos(double);

    virtual QList<PLYDataHeader::Property> getAttributeInfos()const{return QList<PLYDataHeader::Property>();}

protected:

    void setInputType(int input);
    void setStacks(int stacks);
    void setSlices(int slices);

};

#endif // OBJECT3D_H
