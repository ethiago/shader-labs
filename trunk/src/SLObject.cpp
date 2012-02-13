#include "SLObject.h"

SLObject::SLObject(Object3D *obj, QObject *parent) :
    QObject(parent), m_object(obj)
{
}

void SLObject::setObject(Object3D *obj)
{
    if(m_object != NULL)
        delete m_object;

    m_object = obj;
}
