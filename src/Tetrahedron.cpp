#include "Tetrahedron.h"
#include <QVector3D>

#ifdef __APPLE__
    #include <OpenGL/gl.h>
#else
    #include <GL/gl.h>
#endif

Tetrahedron::Tetrahedron(const QVector3D& center):
    Object3D(center)
{
    indexList = glGenLists(1);
    setInputType(GL_TRIANGLES);
    storeList();
}

Tetrahedron::Tetrahedron(const Tetrahedron& tt): Object3D(tt)
{
    indexList = glGenLists(1);

    storeList();
}

Object3D* Tetrahedron::copy() const
{
    return new Tetrahedron(*this);
}

Tetrahedron::~Tetrahedron()
{
    glDeleteLists(indexList,1);
}

void Tetrahedron::drawGeometry(void)const
{
    glCallList(indexList);
}

void Tetrahedron::storeList()const
{


    float v[12] = { -1.0, -1.0, -1.0,
                     1.0,  1.0, -1.0,
                     1.0, -1.0,  1.0,
                    -1.0,  1.0,  1.0 };

    float n[12] = {  1.0,  1.0,  1.0,
                    -1.0, -1.0,  1.0,
                    -1.0,  1.0, -1.0,
                     1.0, -1.0, -1.0 };


    glNewList(indexList, GL_COMPILE);
    {
        glBegin(GL_TRIANGLES);
        {
            glNormal3fv(&n[3*0]);
            glVertex3fv(&v[3*1]);
            glVertex3fv(&v[3*3]);
            glVertex3fv(&v[3*2]);


            glNormal3fv(&n[3*1]);
            glVertex3fv(&v[3*3]);
            glVertex3fv(&v[3*0]);
            glVertex3fv(&v[3*2]);

            glNormal3fv(&n[3*2]);
            glVertex3fv(&v[3*3]);
            glVertex3fv(&v[3*1]);
            glVertex3fv(&v[3*0]);

            glNormal3fv(&n[3*3]);
            glVertex3fv(&v[3*0]);
            glVertex3fv(&v[3*1]);
            glVertex3fv(&v[3*2]);
        }glEnd();
    }glEndList();
}
