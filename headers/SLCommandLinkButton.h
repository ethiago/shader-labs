#ifndef SLCOMMANDLINKBUTTON_H
#define SLCOMMANDLINKBUTTON_H

#include <QCommandLinkButton>
#include <QMouseEvent>

#include "Global.h"

class SLCommandLinkButton : public QCommandLinkButton
{
    Q_OBJECT

public:
    explicit SLCommandLinkButton(ShaderLab::Shader shadertype, QWidget *parent = 0);

private slots:
    void clicou(void);

signals:
    void clicked(ShaderLab::Shader shaderType);

private:
    ShaderLab::Shader shaderType;
};

#endif // SLCOMMANDLINKBUTTON_H
