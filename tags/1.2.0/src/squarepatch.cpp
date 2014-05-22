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

SquarePatch::SquarePatch(int stacks, int slices) :
    Patch()
{
    setStacks(stacks);
    setSlices(slices);
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

    float stepX = 2.0/slices();
    float stepY = 2.0/stacks();

    float stepCX = 1.0/slices();
    float stepCY = 1.0/stacks();

    float norm[3] = { 0.0, 0.0, 1.0 };

    glNormal3fv(norm);

    glBegin(GL_PATCHES);

    float cy = 0.0;
    for(float y = -1.0; y < 1.0; y += stepY, cy += stepCY)
    {
        float cx = 0.0;
        for(float x = -1.0; x < 1.0; x += stepX, cx += stepCX)
        {


            //Vert1
            glTexCoord2f(cx, cy);
            glVertex3f(x, y, 0.0);

            //Vert2
            glTexCoord2f(cx + stepCX, cy);
            glVertex3f(x + stepX, y, 0.0);

            //Vert3
            glTexCoord2f(cx + stepCX, cy + stepCY);
            glVertex3f(x + stepX, y + stepY, 0.0);

            //Vert4
            glTexCoord2f(cx, cy + stepCY);
            glVertex3f(x,y + stepY,0.0);



        }
    }

    glEnd();
}
