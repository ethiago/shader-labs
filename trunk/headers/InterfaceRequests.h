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
    static QString saveProjectAsRequestDialog(void);
    static bool removeFileFromProject(const QString& fileName);
    static bool includeFileIntoProject(const QString& fileName);
    static QString includeNewFileIntoProject(ShaderLab::Shader);
    static bool replaceFileIntoProject(const QString& fileName);
    static bool createProject(void);
    static void openFileProblem(const QString& filename);
    static QString openShader(ShaderLab::Shader shader);
    static QString openProject();
    static QString saveImage();
    static void notLoadProject(void);
    static bool projectSaveContinue(void);


};

#endif // INTERFACEREQUESTS_H
