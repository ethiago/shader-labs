#include "Plane.h"
#include <QtOpenGL>

Plane::Plane(int stacks, int slices) : Object3D()
{
    setStacks(stacks);
    setSlices(slices);

    indexList = glGenLists(1);
    setInputType(GL_TRIANGLES);
    storeList();
}

Plane::Plane(const Plane& p): Object3D(p)
{
    indexList = glGenLists(1);
    storeList();
}

Plane::~Plane()
{
    glDeleteLists(indexList, 1);
}

Object3D* Plane::copy(void) const
{
    return new Plane(*this);
}

void Plane::drawGeometry(void) const
{

    glCallList(indexList);
}

void Plane::storeList() const
{
    float stepX = 2.0/slices();
    float stepY = 2.0/stacks();

    float stepCX = 1.0/slices();
    float stepCY = 1.0/stacks();

    float norm[3] = { 0.0, 0.0, 1.0 };

    glNewList(indexList, GL_COMPILE);

    glNormal3fv(norm);

    glBegin(GL_QUADS);
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
    glEndList();

}
