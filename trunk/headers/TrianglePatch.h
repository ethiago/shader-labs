#ifndef POINTPATCH_H
#define POINTPATCH_H

#include "Patch.h"

class TrianglePatch : public Patch
{
    Q_OBJECT
public:
    explicit TrianglePatch();

    explicit TrianglePatch(const TrianglePatch& p);
     ~TrianglePatch();

    virtual Object3D* copy() const;

private:
    virtual void drawGeometry(void) const;
};

#endif // POINTPATCH_H
