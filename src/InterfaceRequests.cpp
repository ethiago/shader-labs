#include "InterfaceRequests.h"

#include <QObject>
#include <QMessageBox>
#include <QFileDialog>
#include <QFileInfo>

InterfaceRequests::InterfaceRequests()
{
}

InterfaceRequests::OperationState InterfaceRequests::saveRequestDialog(const QString& filename, bool newFile)
{
    QMessageBox::StandardButton ok = QMessageBox::Yes;
    QMessageBox::StandardButton no = QMessageBox::No;
    QMessageBox::StandardButton cancel = QMessageBox::Cancel;
    QMessageBox::StandardButton bt;

    QString msg;

    if(newFile)
        msg = "The file " + filename + " is new.\n Do you want to save?";
    else
        msg = "The file " + filename + " has been modified.\n Do you want to save?";


     bt = QMessageBox::question(0, "Save File", msg,
                               no | ok | cancel,
                               ok);

    if(bt == ok)
        return Yes;
    else if(bt == no)
        return No;
    else
        return Cancel;
}

bool InterfaceRequests::removeFileFromProject(const QString& fileName)
{
    QMessageBox::StandardButton ok = QMessageBox::Yes;
    QMessageBox::StandardButton no = QMessageBox::No;
    QMessageBox::StandardButton bt;

    QString msg = "The file " + fileName + " is not open.\n Do you want to remove from project?";

    bt = QMessageBox::question(0, "Remove File", msg,
                               no | ok,
                               ok);

    if(bt == ok)
        return true;
    else
        return false;
}

bool InterfaceRequests::includeFileIntoProject(const QString& fileName)
{
    QMessageBox::StandardButton ok = QMessageBox::Yes;
    QMessageBox::StandardButton no = QMessageBox::No;
    QMessageBox::StandardButton bt;

    QString msg = "The file " + fileName + " is not within the project.\n Do you want to include it?";

    bt = QMessageBox::question(0, "Include File", msg,
                               no | ok,
                               ok);

    if(bt == ok)
        return true;
    else
        return false;
}

QString InterfaceRequests::includeNewFileIntoProject(ShaderLab::Shader shaderType)
{
    QMessageBox::StandardButton ok = QMessageBox::Yes;
    QMessageBox::StandardButton no = QMessageBox::No;
    QMessageBox::StandardButton bt;

    QString msg = "The new " + ShaderLab::shaderToStr(shaderType) +
            " file is not within the project.\n Do you want to include it?";

    bt = QMessageBox::question(0, "Include File", msg,
                               no | ok,
                               ok);

    if(bt == ok)
    {
        return InterfaceRequests::saveAsRequestDialog(shaderType);
    }
    else
        return QString();
}

bool InterfaceRequests::replaceFileIntoProject(const QString& fileName)
{
    QMessageBox::StandardButton ok = QMessageBox::Yes;
    QMessageBox::StandardButton no = QMessageBox::No;
    QMessageBox::StandardButton bt;

    QString msg = "The file " + fileName + " is diferent within the project.\n Do you want to replace it?";

    bt = QMessageBox::question(0, "Replace File", msg,
                               no | ok,
                               ok);

    if(bt == ok)
        return true;
    else
        return false;
}

bool InterfaceRequests::createProject(void)
{
    QMessageBox::StandardButton ok = QMessageBox::Yes;
    QMessageBox::StandardButton no = QMessageBox::No;
    QMessageBox::StandardButton bt;

    QString msg = "The project was not created.\n Do you want to create?";

    bt = QMessageBox::question(0, "Create Project", msg,
                               no | ok,
                               ok);

    if(bt == ok)
        return true;
    else
        return false;
}

void InterfaceRequests::openFileProblem(const QString& filename)
{
    QMessageBox::warning(0, "Open file error",
                         "The file is not valid or could not be found.\n"
                         +filename);
}

QString InterfaceRequests::openShader(ShaderLab::Shader shader)
{
    static QString dir = ".";
    QString filepath = QFileDialog::getOpenFileName(0,
                              "Open " + ShaderLab::shaderToStr(shader) + " shader",
                              dir,
                              "*" + ShaderLab::shaderToExt(shader));

    if(!filepath.isEmpty())
    {
        QFileInfo fi(filepath);
        dir = fi.absolutePath();
    }

    return filepath;
}

QString InterfaceRequests::openTexture()
{
    static QString dir = ".";
    QString filepath = QFileDialog::getOpenFileName(0, "Open Image", dir, QObject::tr("Images (*.png *.jpg *.tiff *.svg)"));

    if(!filepath.isEmpty())
    {
        QFileInfo fi(filepath);
        dir = fi.absolutePath();
    }

    return filepath;
}

QString InterfaceRequests::openProject()
{
    static QString dir = ".";
    QString filename = QFileDialog::getOpenFileName(0,
                                                    "Open Project",
                                                    dir,
                                                    "*.slp");
    if(!filename.isEmpty())
    {
        QFileInfo fi(filename);
        dir = fi.absolutePath();
    }

    return filename;
}

QString InterfaceRequests::saveAsRequestDialog(ShaderLab::Shader shader)
{
    static QString dir = ".";
    QString filename = QFileDialog::getSaveFileName(
                       0,
                       "Save " + ShaderLab::shaderToStr(shader) +" shader as...",
                       dir,
                       "*"+ShaderLab::shaderToExt(shader));

    QString ext = ShaderLab::shaderToExt(shader);

    if(filename == ext || filename.isEmpty())
        return QString();

    QFileInfo fi(filename);
    dir = fi.absolutePath();

    if(filename.right(ext.length()) != ext)
        filename += ext;

    return filename;
}

QString InterfaceRequests::saveProjectAsRequestDialog()
{
    static QString dir = ".";
    QString filename = QFileDialog::getSaveFileName(
                       0,
                       "Save project as...",
                       dir,
                       "*.slp");

    QString ext = ".slp";

    if(filename == ext || filename.isEmpty())
        return QString();

    QFileInfo fi(filename);
    dir = fi.absolutePath();

    if(filename.right(ext.length()) != ext)
        filename += ext;

    return filename;
}

QString InterfaceRequests::saveImage()
{
    static QString dir = ".";
    QString filename = QFileDialog::getSaveFileName(0,
                                                    "Save Image",
                                                    dir,
                                                    "*.png");
    QString ext = ".png";

    if(filename == ext || filename.isEmpty())
        return QString();

    QFileInfo fi(filename);
    dir = fi.absolutePath();

    if(filename.right(ext.length()) != ext)
        filename += ext;

    return filename;
}

void InterfaceRequests::notLoadProject()
{
    QMessageBox::warning(0, "Open file error",
                         "The file content was not recognized as ShaderLabs Project.\n");
}

bool InterfaceRequests::projectSaveContinue(void)
{
    QMessageBox::StandardButton ok = QMessageBox::Yes;
    QMessageBox::StandardButton no = QMessageBox::No;
    QMessageBox::StandardButton bt;

    QString msg = QString("There is any new code. New codes will not be included in project.\n") +
            ("Do you want to continue?");

    bt = QMessageBox::question(0, "Save project", msg,
                               no | ok,
                               ok);

    if(bt == ok)
        return true;
    else
        return false;
}

void InterfaceRequests::sizeFileNotMatch(int size, int inf)
{
    QMessageBox::warning(0, "Open file error",
                         "The file size(" + QString::number(size) +
                         ") does not match informed data("+ QString::number(inf) +").");
}

void InterfaceRequests::fileTooLarge(int texelQuantity)
{
    QMessageBox::warning(0, "Open file error",
                         "The number of texels (" + QString::number(texelQuantity) +
                         ") is too large. It must be at most "+ QString::number(5832000) +".");
}
