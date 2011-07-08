#include "Plane.h"
#include <QtOpenGL>

Plane::Plane()
{
    indexList = glGenLists(1);

    //storeList();
}

void Plane::drawGeometry(void) const
{
    //glCallList(indexList);
    storeList();
}

void Plane::storeList() const
{
    float stepX = 2.0/slices();
    float stepY = 2.0/stacks();

    float stepCX = 1.0/slices();
    float stepCY = 1.0/stacks();

    float norm[3] = { 0.0, 0.0, -1.0 };

    //glNewList(indexList, GL_COMPILE);

    glNormal3fv(norm);
    float cx = 0.0;
    for(float x = -1.0; x < 1.0; x += stepX, cx += stepCX)
    {
        float cy = 0.0;
        for(float y = -1.0; y < 1.0; y += stepY, cy += stepCY)
        {
            glBegin(GL_QUADS);


            //Vert4
            glTexCoord2f(cx, cy + stepCY);
            glVertex3f(x,y + stepY,0.0);

            //Vert3
            glTexCoord2f(cx + stepCX, cy + stepCY);
            glVertex3f(x + stepX, y + stepY, 0.0);

            //Vert2
            glTexCoord2f(cx + stepCX, cy);
            glVertex3f(x + stepX, y, 0.0);

            //Vert1
            glTexCoord2f(cx, cy);
            glVertex3f(x, y, 0.0);

            glEnd();
        }
    }

    //glEndList();

}
