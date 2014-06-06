#include "slhemesh.h"
#include "face.h"
#include "halfedge.h"
#include "vertex.h"
#include "slgl3w.h"


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

    QList<QPair<GLuint, QVariant::Type> > attInfo = m_data->getAttribInfo();

    indexList = glGenLists(1);

    glNewList(indexList, GL_COMPILE);


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
            const Vertex *v = (*it)->origin();
            QVector4D p = v->geometry();

            for(int j = 0; j < attInfo.size(); ++j)
            {
                bindAttrValue(attInfo[0].first, v->attribValue(j), attInfo[0].second);
            }

            glVertex4fv(reinterpret_cast<const GLfloat*>(&p));
        }

        glEnd();
    }

    glEndList();
}

void SLHEMesh::bindAttrValue(unsigned int attrLoc, const QList<QVariant>& list, QVariant::Type type)const
{
    if(attrLoc <= 0)
        return;

    if(type != QVariant::Int && type != QVariant::Double)
        return;

    if(list.size() == 0 || list.size() > 4)
        return;

    if(type == QVariant::Int)
    {
        switch(list.size())
        {
        case 1:
            glVertexAttribI1iEXT(attrLoc, list[0].value<int>());
            break;
        case 2:
            glVertexAttribI2iEXT(attrLoc, list[0].value<int>(), list[1].value<int>());
            break;
        case 3:
            glVertexAttribI3iEXT(attrLoc, list[0].value<int>(), list[1].value<int>(), list[2].value<int>());
            break;
        case 4:
            glVertexAttribI4iEXT(attrLoc, list[0].value<int>(), list[1].value<int>(), list[2].value<int>(), list[3].value<int>());
            break;
        }
    }else
    {
        switch(list.size())
        {
        case 1:
            glVertexAttrib1f(attrLoc, list[0].value<float>());
            break;
        case 2:
            glVertexAttrib2f(attrLoc, list[0].value<float>(), list[1].value<float>());
            break;
        case 3:
            glVertexAttrib3f(attrLoc, list[0].value<float>(), list[1].value<float>(), list[2].value<float>());
            break;
        case 4:
            glVertexAttrib4f(attrLoc, list[0].value<float>(), list[1].value<float>(), list[2].value<float>(), list[3].value<float>());
            break;
        }
    }
}

void SLHEMesh::afterLink(GLuint programID)
{
    QStringList attNames = m_data->getAttribNames();
    for(int i = 0; i < attNames.size(); ++i)
    {
        int loc = SLGl3W::getAttributeLocation(programID, attNames[i]);
        m_data->setAttribLocation(i, loc);
    }
}
