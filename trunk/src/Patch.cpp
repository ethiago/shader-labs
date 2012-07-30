#include "Patch.h"
#include <QtOpenGL>

Patch::Patch() :
    Object3D()
{
    setInputType(GL_TRIANGLES);
}

Patch::Patch(const Patch& p) :
    Object3D(p)
{
}
