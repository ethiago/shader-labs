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
    static OperationState removeFileFromProject(const QString& fileName);
    static OperationState includeFileIntoProject(const QString& fileName);
    static OperationState replaceFileIntoProject(const QString& fileName);
    static bool createProject(void);
    static void openFileProblem(const QString& filename);
    static QString openShader(ShaderLab::Shader shader);
    static QString openTexture();
    static QString openProject();
    static QString saveImage();
    static void notLoadProject(void);
    static bool projectSaveContinue(void);
    static void sizeFileNotMatch(int size, int inf);
    static void fileTooLarge(int texelQuantity);
    static void thereIsAlreadyAnOpenedShader(ShaderLab::Shader shader);
    static void thereIsAlreadyAnOpenedProject();
    static OperationState notSavedCodeContinueAsk();
    static void notSavedCode();
    static OperationState openedProjectContinueAsk();
    static void openedCode_Project();
    static OperationState projectDifferences();


};

#endif // INTERFACEREQUESTS_H
