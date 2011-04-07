#include "headers/commandlinkbutton.h"

CommandLinkButton::CommandLinkButton(ShaderLab::Shader shadertype, QWidget *parent) :
    QCommandLinkButton(parent)
{
    shaderType = shadertype;
    setText( tr(QString(ShaderLab::shaderToStr(shadertype) + " Code").toAscii()) );
}

void CommandLinkButton::mouseReleaseEvent(QMouseEvent *event)
{
    if(event->button() == Qt::LeftButton)
        emit clicked(shaderType);
}
