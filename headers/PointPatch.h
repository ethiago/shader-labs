#ifndef POINTPATCH_H
#define POINTPATCH_H

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>
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
