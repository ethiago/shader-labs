#include "squarepatch.h"
#include "slgl3w.h"

#ifdef __APPLE__
    #include <OpenGL/gl.h>
    #include <OpenGL/glext.h>
#else
    #define GL_GLEXT_PROTOTYPES 1
    #include <GL/gl.h>
    #include <GL/glext.h>
#endif

SquarePatch::SquarePatch() :
    Patch()
{
}

SquarePatch::SquarePatch(const SquarePatch& p) :
    Patch(p)
{

}

SquarePatch::~SquarePatch()
{
}

Object3D* SquarePatch::copy() const
{
    return new SquarePatch(*this);
}

void SquarePatch::drawGeometry(void) const
{
    SLGl3W::setPatchVertices(4);

    glBegin(GL_PATCHES);
    glVertex3i(-1,-1,0);
    glVertex3i( 1,-1,0);
    glVertex3i( 1, 1,0);
    glVertex3i(-1, 1,0);
    glEnd();
}
