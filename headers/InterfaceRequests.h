#ifndef INTERFACEREQUESTS_H
#define INTERFACEREQUESTS_H

#include <QString>
#include "Global.h"

class InterfaceRequests
{
    InterfaceRequests();

public:
    enum OperationState{ Yes, No, Cancel };

    static OperationState saveRequestDialog(const QString& filename, bool newFile);
    static QString saveAsRequestDialog(ShaderLab::Shader shader);

};

#endif // INTERFACEREQUESTS_H
