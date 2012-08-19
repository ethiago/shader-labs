#include "IcosahedronPatch.h"
#include "Global.h"
#include <QtOpenGL>
#include <QVector3D>

IcosahedronPatch::IcosahedronPatch():
    Patch()
{
    indexList = glGenLists(1);
    setInputType(GL_TRIANGLES);
    storeList();
}

IcosahedronPatch::IcosahedronPatch(const IcosahedronPatch& cb): Patch(cb)
{
    indexList = glGenLists(1);
    setInputType(GL_TRIANGLES);
    storeList();
}

Object3D* IcosahedronPatch::copy(void) const
{
    return new IcosahedronPatch(*this);
}

IcosahedronPatch::~IcosahedronPatch()
{
    glDeleteLists(indexList,1);
}

void IcosahedronPatch::drawGeometry(void)const
{
    glCallList(indexList);
}

void IcosahedronPatch::storeList()const
{

    float v[12*3] = {   0.0, -0.525731, 0.850651,
                      0.850651, 0, 0.525731,
                      0.850651, 0, -0.525731,
                      -0.850651, 0, -0.525731,
                      -0.850651, 0, 0.525731,
                      -0.525731, 0.850651, 0,
                      0.525731, 0.850651, 0,
                      0.525731, -0.850651, 0,
                      -0.525731, -0.850651, 0,
                      0, -0.525731, -0.850651,
                      0, 0.525731, -0.850651,
                      0, 0.525731, 0.850651
     };

    int tri[20*3] = { 1,2,6,
                      1,7,2,
                      3,4,5,
                      4,3,8,
                      6,5,11,
                      5,6,10,
                      9,10,2,
                      10,9,3,
                      7,8,9,
                      8,7,0,
                      11,0,1,
                      0,11,4,
                      6,2,10,
                      1,6,11,
                      3,5,10,
                      5,4,11,
                      2,7,9,
                      7,1,0,
                      3,9,8,
                      4,8,0
   };

  glNewList(indexList, GL_COMPILE);
  {
      glBegin(GL_PATCHES);
      {
          for(int i = 0; i < 20; ++i)
          {
              int t = i*3;
              for(int j = 0; j < 3; ++j)
              {
                  int vi = tri[t+j]*3;
                  glNormal3fv(&(v[vi]));
                  glVertex3fv(&(v[vi]));
              }
          }
      }glEnd();
  }glEndList();
}
