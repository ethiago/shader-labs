#ifndef SQUAREPATCH_H
#define SQUAREPATCH_H

#include "Patch.h"

class SquarePatch : public Patch
{
    Q_OBJECT
public:
    explicit SquarePatch(int stacks = 1, int slices = 1);

    explicit SquarePatch(const SquarePatch& p);
     ~SquarePatch();

    virtual Object3D* copy() const;

private:
    virtual void drawGeometry(void) const;
};

#endif // SQUAREPATCH_H
