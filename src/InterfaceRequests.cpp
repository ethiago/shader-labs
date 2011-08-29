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

    if(filename == ShaderLab::shaderToExt(shader))
        return QString();

    return filename;
}

QString InterfaceRequests::saveProjectAsRequestDialog()
{
    QString filename = QFileDialog::getSaveFileName(
                       0,
                       "Save project as...",
                       "../..",
                       "*.slp");

    if(filename == ".slp")
        return QString();

    return filename;
}
