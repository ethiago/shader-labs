#ifndef ICOSAHEDRONPATCH_H
#define ICOSAHEDRONPATCH_H

#include "Patch.h"

class IcosahedronPatch : public Patch
{
    int indexList;
public:
    explicit IcosahedronPatch();
    explicit IcosahedronPatch(const IcosahedronPatch& sph);
    ~IcosahedronPatch();

    virtual Object3D* copy(void) const;

private:
    virtual void drawGeometry(void) const;

    void storeList()const;

};

#endif // ICOSAHEDRONPATCH_H
