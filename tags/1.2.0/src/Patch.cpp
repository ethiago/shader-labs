#include "Patch.h"

#ifdef __APPLE__
    #include <OpenGL/gl.h>
    #include <OpenGL/glext.h>
#else
    #define GL_GLEXT_PROTOTYPES 1
    #include <GL/gl.h>
    #include <GL/glext.h>
#endif

Patch::Patch() :
    Object3D()
{
    setInputType(GL_TRIANGLES);
}

Patch::Patch(const Patch& p) :
    Object3D(p)
{
}
