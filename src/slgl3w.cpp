#include "gl3w.h"
#include "slgl3w.h"

SLGl3W::SLGl3W()
{
}

void SLGl3W::setPatchVertices(int n)
{
    glPatchParameteri(GL_PATCH_VERTICES, n);
}
