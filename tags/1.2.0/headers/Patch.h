#ifndef PATCH_H
#define PATCH_H

#include "Object3D.h"

#ifndef GL_PATCHES
#define GL_PATCHES            0xE
#endif

#ifndef GL_PATCH_VERTICES
#define GL_PATCH_VERTICES     0x8E72
#endif

class Patch : public Object3D
{
    Q_OBJECT
public:
    explicit Patch();
    explicit Patch(const Patch&);

};

#endif // PACH_H
