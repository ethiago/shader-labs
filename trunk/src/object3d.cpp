#include "object3d.h"

Object3D::Object3D(const QVector3D& center, QObject *parent)
    : QObject(parent), m_center(center), m_quaternion(QQuaternion()) ,
      m_slices(10), m_stacks (10)
{
}

Object3D::Object3D(const Object3D& obj)
    : QObject(obj.parent()), m_center(obj.center()),
      m_quaternion(obj.quaternion()),
      m_slices(obj.slices()), m_stacks(obj.stacks())
{
}

void Object3D::setQuaternion(const QQuaternion& quaternion)
{
    m_quaternion = quaternion;
}

const QQuaternion& Object3D::quaternion(void)const
{
    return m_quaternion;
}

void Object3D::setCenter(const QVector3D& center)
{
    m_center = center;
}

const QVector3D& Object3D::center(void)const
{
    return m_center;
}

void Object3D::setSlices(int slices)
{
    m_slices = slices;
}

int Object3D::slices(void)const
{
    return m_slices;
}

void Object3D::setStacks(int stacks)
{
    m_stacks = stacks;
}

int Object3D::stacks(void)const
{
    return m_stacks;
}
