#include <QtOpenGL>
#include <QVector3D>
#include "Cube.h"
#include "Global.h"

Cube::Cube(const QVector3D& center, QObject *parent):
    Object3D(center, parent)
{
    indexList = glGenLists(1);

    storeList();
}

Cube::Cube(const Cube& cb): Object3D(cb)
{
    indexList = glGenLists(1);

    storeList();
}

Cube::~Cube()
{
    glDeleteLists(indexList,1);
}

void Cube::drawGeometry(void)const
{
    glCallList(indexList);
}

void Cube::storeList()const
{

    float v[24] = {-1.0, -1.0, -1.0,
                    1.0, -1.0, -1.0,
                   -1.0,  1.0, -1.0,
                    1.0,  1.0, -1.0,
                   -1.0,  1.0,  1.0,
                    1.0,  1.0,  1.0,
                   -1.0, -1.0,  1.0,
                    1.0, -1.0,  1.0};

    float n[24] = {-0.57735, -0.57735, -0.57735,
                    0.57735, -0.57735, -0.57735,
                   -0.57735,  0.57735, -0.57735,
                    0.57735,  0.57735, -0.57735,
                   -0.57735,  0.57735,  0.57735,
                    0.57735,  0.57735,  0.57735,
                   -0.57735, -0.57735,  0.57735,
                    0.57735, -0.57735,  0.57735};


    glNewList(indexList, GL_COMPILE);
    {
        glBegin(GL_QUAD_STRIP);
        {
            glNormal3fv(&n[3*0]);
            glVertex3fv(&v[3*0]);

            glNormal3fv(&n[3*1]);
            glVertex3fv(&v[3*1]);

            glNormal3fv(&n[3*2]);
            glVertex3fv(&v[3*2]);

            glNormal3fv(&n[3*3]);
            glVertex3fv(&v[3*3]);

            glNormal3fv(&n[3*4]);
            glVertex3fv(&v[3*4]);

            glNormal3fv(&n[3*5]);
            glVertex3fv(&v[3*5]);

            glNormal3fv(&n[3*6]);
            glVertex3fv(&v[3*6]);

            glNormal3fv(&n[3*7]);
            glVertex3fv(&v[3*7]);

            glNormal3fv(&n[3*0]);
            glVertex3fv(&v[3*0]);

            glNormal3fv(&n[3*1]);
            glVertex3fv(&v[3*1]);
        }glEnd();

        glBegin(GL_QUADS);
        {
            glNormal3fv(&n[3*1]);
            glVertex3fv(&v[3*1]);

            glNormal3fv(&n[3*3]);
            glVertex3fv(&v[3*3]);

            glNormal3fv(&n[3*5]);
            glVertex3fv(&v[3*5]);

            glNormal3fv(&n[3*7]);
            glVertex3fv(&v[3*7]);

            glNormal3fv(&n[3*0]);
            glVertex3fv(&v[3*0]);

            glNormal3fv(&n[3*2]);
            glVertex3fv(&v[3*2]);

            glNormal3fv(&n[3*4]);
            glVertex3fv(&v[3*4]);

            glNormal3fv(&n[3*6]);
            glVertex3fv(&v[3*6]);

        }glEnd();

    }glEndList();
}
