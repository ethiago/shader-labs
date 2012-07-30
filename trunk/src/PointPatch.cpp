#include "PointPatch.h"
#include <QtOpenGL>

PointPatch::PointPatch() :
    Patch()
{
}

PointPatch::PointPatch(const PointPatch& p) :
    Patch(p)
{

}

PointPatch::~PointPatch()
{
}

Object3D* PointPatch::copy() const
{
    return new PointPatch(*this);
}

void PointPatch::drawGeometry(void) const
{
    //gl_PatchParameteri(GL_PATCH_VERTICES, 3);

    glBegin(GL_PATCHES);
    glVertex3i(0,0,0);
    glVertex3i(1,0,0);
    glVertex3i(0,1,0);
    glEnd();
}
