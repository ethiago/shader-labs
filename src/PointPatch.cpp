#include "PointPatch.h"

#ifdef __APPLE__
    #include <OpenGL/gl.h>
    #include <OpenGL/glext.h>
#else
    #define GL_GLEXT_PROTOTYPES 1
    #include <GL/gl.h>
    #include <GL/glext.h>
#endif

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
    //glPatchParameteri(GL_PATCH_VERTICES, 3);

    glBegin(GL_PATCHES);
    glVertex3i(0,0,0);
    glVertex3i(1,0,0);
    glVertex3i(0,1,0);
    glEnd();
}
