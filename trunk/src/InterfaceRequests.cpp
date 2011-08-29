#include "InterfaceRequests.h"

#include <QMessageBox>
#include <QFileDialog>

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

QString InterfaceRequests::saveAsRequestDialog(ShaderLab::Shader shader)
{
    QString filename = QFileDialog::getSaveFileName(
                       0,
                       "Save " + ShaderLab::shaderToStr(shader) +" shader as...",
                       "../..",
                       ShaderLab::shaderToExt(shader));

    QString ext = ShaderLab::shaderToExt(shader);

    if(filename == ext)
        return QString();

    if(filename.right(ext.length()) != ext)
        filename += ext;

    return filename;
}

QString InterfaceRequests::saveProjectAsRequestDialog()
{
    QString filename = QFileDialog::getSaveFileName(
                       0,
                       "Save project as...",
                       "../..",
                       "*.slp");

    QString ext = ".slp";

    if(filename == ext)
        return QString();

    if(filename.right(ext.length()) != ext)
        filename += ext;

    return filename;
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
