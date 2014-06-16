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

    int addAttribute(const PLYDataHeader::Property& p);
    int addFaceUniform(const PLYDataHeader::Property& p);
    int addUniform(const PLYDataHeader::Property& p);

    int findAttribute(const QString& p);

    const QList<PLYDataHeader::Property>& getAttributeInfos()const;
    const QList<PLYDataHeader::Property>& getFaceUniformInfos()const;
    const QList<PLYDataHeader::Property>& getUniformInfos()const;

    void setAttributeLocation(int idx, int location);
    void setFaceUniformLocation(int idx, int location);
    void setUniformLocation(int idx, int location);


    virtual void afterLink(unsigned int programId);

protected:

    void setInputType(int input);
    void setStacks(int stacks);
    void setSlices(int slices);

    QList<PLYDataHeader::Property> m_attributeInfo;
    QList<PLYDataHeader::Property> m_faceInfo;
    QList<PLYDataHeader::Property> m_uniformInfo;

    QList<int> m_attLocation;
    QList<int> m_faceLocation;
    QList<int> m_uniformLocation;

    void bindAttribute(int i, const QList<QVariant>& list );

    void bindUniform(int i, const QList<QVariant>& list, PLYDataHeader::Property::Type type);

};

#endif // OBJECT3D_H
