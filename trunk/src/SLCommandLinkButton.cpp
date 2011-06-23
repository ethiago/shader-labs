#include "SLCommandLinkButton.h"

SLCommandLinkButton::SLCommandLinkButton(ShaderLab::Shader shadertype, QWidget *parent) : QCommandLinkButton(parent)
{
    shaderType = shadertype;
    setText( tr(QString(ShaderLab::shaderToStrCap(shadertype) + " code").toAscii()) );

    connect(this, SIGNAL(clicked()), this, SLOT(clicou()));
}

/* Tá em português!!!!! */
void SLCommandLinkButton::clicou(void)
{
    emit clicked(shaderType);
}
