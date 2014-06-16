#include "slhemesh.h"
#include "face.h"
#include "halfedge.h"
#include "vertex.h"
#include "slgl3w.h"
#include <QtOpenGL>


using namespace heds;

SLHEMesh::SLHEMesh(const QString &fileName) :
    Object3D(), m_fileName(fileName), indexList(0)
{
}

SLHEMesh::SLHEMesh(const SLHEMesh& cp) :
    Object3D(cp), m_fileName(cp.fileName()), indexList(0), m_data(cp.m_data)
{
    storeList();
}

SLHEMesh::~SLHEMesh()
{
    if(indexList > 0)
        glDeleteLists(indexList, 1);
}

const QString& SLHEMesh::fileName()const
{
    return m_fileName;
}

Object3D* SLHEMesh::copy(void) const
{
    return new SLHEMesh(*this);
}

void SLHEMesh::drawGeometry(void) const
{
    glCallList(indexList);
}

void SLHEMesh::afterLink(unsigned int programId)
{
    //glUseProgram(programId);
    storeList();
    //glUseProgram(0);
}

void SLHEMesh::storeList()
{
    if(indexList > 0)
        glDeleteLists(indexList, 1);

    indexList = glGenLists(1);

    glNewList(indexList, GL_COMPILE);

    for(int i = 0; i < m_uniformInfo.size(); ++i)
    {
        bindUniform(m_uniformLocation[i], m_data.uniformValue(i), m_uniformInfo[i].type());
    }

    for(int i = 0; i < m_data.numberOfFaces(); ++i)
    {
        Face *f = m_data.getFace(i);

        for(int j = 0; j < m_faceInfo.size(); ++j)
        {
            bindUniform(m_faceLocation[j], f->uniformValue(j), m_faceInfo[j].type());
        }

        int nV = f->numberOfVertices();

        if(nV < 3)
            continue;

        if(nV == 3)
            glBegin(GL_TRIANGLES);
        else if(nV == 4)
            glBegin(GL_QUADS);
        else
            glBegin(GL_POLYGON);

        QVector3D n = f->getNormal();
        n.normalize();

        Face::iterator it;
        for(it = f->begin(); it != f->end(); ++it)
        {
            const Vertex *v = (*it)->origin();

            for(int j = 0; j < m_attributeInfo.size(); ++j)
            {
                bindAttribute(j, v->attribValue(j));
            }

            QVector2D tc = v->texCoord();
            glTexCoord2fv(reinterpret_cast<const GLfloat*>(&tc));

            QVector3D nv = v->normal();
            if(!nv.isNull())
                glNormal3fv(reinterpret_cast<const GLfloat*>(&nv));
            else
                glNormal3fv(reinterpret_cast<const GLfloat*>(&n));

            QVector4D p = v->geometry();
            glVertex4fv(reinterpret_cast<const GLfloat*>(&p));
        }

        glEnd();
    }

    glDisable(GL_LIGHTING);
    glColor3f(0.451,1.0,0.031);
    for(int i = 0; i < m_data.numberOfBondaries(); ++i)
    {
        glBegin(GL_LINE_STRIP);

        const HalfEdge * he = m_data.getBoundaryReferenceEdge(i);
        QVector4D p = he->origin()->geometry();
        glVertex4fv(reinterpret_cast<const GLfloat*>(&p));
        const HalfEdge * next = he->next();
        while(next != he)
        {
            QVector4D p = next->origin()->geometry();
            glVertex4fv(reinterpret_cast<const GLfloat*>(&p));
            next = next->next();
        }
        glVertex4fv(reinterpret_cast<const GLfloat*>(&p));

        glEnd();
    }
    glEnable(GL_LIGHTING);

    glEndList();
}

heds::HalfEdgeData * SLHEMesh::data()
{
    return &m_data;
}
