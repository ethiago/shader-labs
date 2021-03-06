#include "TrianglePatch.h"
#include "slgl3w.h"

#ifdef __APPLE__
    #include <OpenGL/gl.h>
    #include <OpenGL/glext.h>
#else
    #define GL_GLEXT_PROTOTYPES 1
    #include <GL/gl.h>
    #include <GL/glext.h>
#endif

TrianglePatch::TrianglePatch() :
    Patch()
{
}

TrianglePatch::TrianglePatch(const TrianglePatch& p) :
    Patch(p)
{

}

TrianglePatch::~TrianglePatch()
{
}

Object3D* TrianglePatch::copy() const
{
    return new TrianglePatch(*this);
}

void TrianglePatch::drawGeometry(void) const
{
    SLGl3W::setPatchVertices(3);

    glBegin(GL_PATCHES);
    glVertex3i(-1,0,0);
    glVertex3i(1,0,0);
    glVertex3i(0,1,0);
    glEnd();
}
