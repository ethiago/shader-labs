#include "slhemesh.h"
#include "face.h"
#include "halfedge.h"
#include "vertex.h"

#include <QtOpenGL>

using namespace heds;

SLHEMesh::SLHEMesh(HalfEdgeData * data, bool mydata) : Object3D(), indexList(0), m_data(data), myData(mydata)
{
}

SLHEMesh::SLHEMesh(const SLHEMesh& cp) : Object3D(cp), indexList(cp.indexList), m_data(cp.m_data), myData(false)
{
}

SLHEMesh::~SLHEMesh()
{
    if(myData)
    {
        delete m_data;
        if(indexList > 0)
            glDeleteLists(indexList, 1);
    }
}

Object3D* SLHEMesh::copy(void) const
{
    return new SLHEMesh(*this);
}

void SLHEMesh::drawGeometry(void) const
{
    glCallList(indexList);
}

void SLHEMesh::storeList()
{
    if(indexList > 0)
        glDeleteLists(indexList, 1);

    indexList = glGenLists(1);

    glNewList(indexList, GL_COMPILE);

    //glBegin(GL_QUADS);
    for(int i = 0; i < m_data->numberOfFaces(); ++i)
    {
        Face *f = m_data->getFace(i);

        int nV = f->numberOfVertices();

        if(nV < 3)
            continue;

        if(nV == 3)
            glBegin(GL_TRIANGLES);
        else if(nV == 4)
            glBegin(GL_QUADS);
        else
            glBegin(GL_POLYGON);

        Face::iterator it;
        QVector3D n = f->getNormal();
        glNormal3fv(reinterpret_cast<const GLfloat*>(&n));
        for(it = f->begin(); it != f->end(); ++it)
        {
            QVector4D v = (*it)->origin()->geometry();
            glVertex4fv(reinterpret_cast<const GLfloat*>(&v));
        }

        glEnd();
    }

    glEndList();
}
