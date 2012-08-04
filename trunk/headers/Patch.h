#ifndef PATCH_H
#define PATCH_H

#define GL_GLEXT_PROTOTYPES
#include <GL/gl.h>
#include <GL/glext.h>

#include "Object3D.h"

class Patch : public Object3D
{
    Q_OBJECT
public:
    explicit Patch();
    explicit Patch(const Patch&);

};

#endif // PACH_H
