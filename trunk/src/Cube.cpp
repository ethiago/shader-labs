#include "Cube.h"
#include <QVector3D>

#ifdef __APPLE__
    #include <OpenGL/gl.h>
#else
    #include <GL/gl.h>
#endif

Cube::Cube(const QVector3D& center):
    Object3D(center)
{
    indexList = glGenLists(1);
    setInputType(GL_TRIANGLES);
    storeList();
}

Cube::Cube(const Cube& cb): Object3D(cb)
{
    indexList = glGenLists(1);
    storeList();
}

Object3D* Cube::copy(void) const
{
    return new Cube(*this);
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

    ////////////////////////////////////////////////////////////
    float v[24] = {   -1.0, -1.0,  1.0,
                       1.0, -1.0,  1.0,
                      -1.0,  1.0,  1.0,
                       1.0,  1.0,  1.0,
                      -1.0,  1.0, -1.0,
                       1.0,  1.0, -1.0,
                      -1.0, -1.0, -1.0,
                       1.0, -1.0, -1.0};

    float t[20] = { 0.0,  0.66,
                    0.0,  0.33,
                    0.25, 0.66,
                    0.25, 0.33,
                    0.5 , 0.66,
                    0.5 , 0.33,
                    0.75, 0.66,
                    0.75, 0.33,
                    1.0 , 0.66,
                    1.0 , 0.33
                   };
    float t2[16]= { 0.25, 1.0,
                    0.25, 0.0,
                    0.25, 0.66,
                    0.25, 0.33,
                    0.5 , 0.66,
                    0.5 , 0.33,
                    0.5 , 1.0,
                    0.5 , 0.0
                 };


  glColor4f( 0.35, 0.35, 0.35, 1.0);

  glNewList(indexList, GL_COMPILE);
  {
      glBegin(GL_QUADS);
      {
          //F1
          glNormal3f(0.0,0.0,1.0);
          glTexCoord2fv(&t[2*0]);
          glVertex3fv(&v[3*0]);

          glTexCoord2fv(&t[2*1]);
          glVertex3fv(&v[3*1]);

          glTexCoord2fv(&t[2*3]);
          glVertex3fv(&v[3*3]);

          glTexCoord2fv(&t[2*2]);
          glVertex3fv(&v[3*2]);


          //F4
          glNormal3f(0.0,1.0,0.0);
          glTexCoord2fv(&t[2*2]);
          glVertex3fv(&v[3*2]);

          glTexCoord2fv(&t[2*3]);
          glVertex3fv(&v[3*3]);

          glTexCoord2fv(&t[2*5]);
          glVertex3fv(&v[3*5]);

          glTexCoord2fv(&t[2*4]);
          glVertex3fv(&v[3*4]);


          //F0
          glNormal3f(0.0,0.0,-1.0);
          glTexCoord2fv(&t[2*4]);
          glVertex3fv(&v[3*4]);

          glTexCoord2fv(&t[2*5]);
          glVertex3fv(&v[3*5]);

          glTexCoord2fv(&t[2*7]);
          glVertex3fv(&v[3*7]);

          glTexCoord2fv(&t[2*6]);
          glVertex3fv(&v[3*6]);


          //F5
          glNormal3f(0.0,-1.0,0.0);
          glTexCoord2fv(&t[2*6]);
          glVertex3fv(&v[3*6]);

          glTexCoord2fv(&t[2*7]);
          glVertex3fv(&v[3*7]);

          glTexCoord2fv(&t[2*9]);
          glVertex3fv(&v[3*1]);

          glTexCoord2fv(&t[2*8]);
          glVertex3fv(&v[3*0]);


          //F2
          glNormal3f(1.0,0.0,0.0);
          glTexCoord2fv(&t2[2*7]);
          glVertex3fv(&v[3*7]);

          glTexCoord2fv(&t2[2*5]);
          glVertex3fv(&v[3*5]);

          glTexCoord2fv(&t2[2*3]);
          glVertex3fv(&v[3*3]);

          glTexCoord2fv(&t2[2*1]);
          glVertex3fv(&v[3*1]);

          //F3
          glNormal3f(-1.0,0.0,0.0);
          glTexCoord2fv(&t2[2*0]);
          glVertex3fv(&v[3*0]);

          glTexCoord2fv(&t2[2*2]);
          glVertex3fv(&v[3*2]);

          glTexCoord2fv(&t2[2*4]);
          glVertex3fv(&v[3*4]);

          glTexCoord2fv(&t2[2*6]);
          glVertex3fv(&v[3*6]);

      }glEnd();
  }glEndList();

}
