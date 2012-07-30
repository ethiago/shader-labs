#ifndef POINTPATCH_H
#define POINTPATCH_H

#include "Patch.h"

class PointPatch : public Patch
{
    Q_OBJECT
public:
    explicit PointPatch();

    explicit PointPatch(const PointPatch& p);
     ~PointPatch();

    virtual Object3D* copy() const;

private:
    virtual void drawGeometry(void) const;
};

#endif // POINTPATCH_H
