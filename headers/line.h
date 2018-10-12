#ifndef LINE_H
#define LINE_H

#include "Object3D.h"

class Line  : public Object3D
{
    int indexList;
public:
    explicit Line();
    explicit Line(const Line& rhs);
    ~Line();

    virtual Object3D* copy() const;

private:
    virtual void drawGeometry(void) const;

    void storeList();
};

#endif // LINE_H
