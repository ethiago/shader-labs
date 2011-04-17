#include <QDebug>

#include "commandlinkbutton.h"


CommandLinkButton::CommandLinkButton(ShaderLab::Shader shadertype, QWidget *parent) : QCommandLinkButton(parent)
{
    shaderType = shadertype;
    setText( tr(QString(ShaderLab::shaderToStr(shadertype) + " Code").toAscii()) );

    connect(this, SIGNAL(clicked()), this, SLOT(clicou()));
}

/* Tá em português!!!!! */
void CommandLinkButton::clicou(void)
{
    emit clicked(shaderType);
}
