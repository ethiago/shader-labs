#ifndef SLHEMESH_H
#define SLHEMESH_H

#include "Object3D.h"
#include "halfedgedata.h"


class SLHEMesh : public Object3D
{
    int indexList;

    heds::HalfEdgeData * m_data;
    const bool myData;

    void drawGeometry(void) const;

    void bindAttrValue(unsigned int attrLoc, const QList<QVariant>& list, PLYDataHeader::Property::Type type)const;

public:
    SLHEMesh(heds::HalfEdgeData * data, bool mydata = true);
    SLHEMesh(const SLHEMesh& cp);
    ~SLHEMesh();

    Object3D* copy(void) const;

    void storeList();

    QList<PLYDataHeader::Property> getAttributeInfos()const;

private slots:
    void afterLink(GLuint programID);

};

#endif // SLHEMESH_H
