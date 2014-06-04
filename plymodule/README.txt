//PLY MODULE: https://code.google.com/p/plymodule/



//************ Example ***************//

#include <QString>
#include "plyio.h"

int main(int argc, char *argv[])
{

    dir = QString("../plymodule/models/");
    QString model("bun_zipper.ply");
    
    PLYIO plyio1;
    Q_ASSERT(plyio1.load(dir+model));

    Q_ASSERT(plyio1.write("model.ply"));

    PLYIO plyio2;
    Q_ASSERT(plyio2.load("model.ply"));

    PLYData plydata1(plyio1.getData());
    PLYData plydata2(plyio2.getData());
    Q_ASSERT( plydata1 == plydata2 );

    return 0;
}
