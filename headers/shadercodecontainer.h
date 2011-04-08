#ifndef SHADERCODECONTAINER_H
#define SHADERCODECONTAINER_H

#include <QWidget>
#include "global.h"

namespace Ui {
    class ShaderCodeContainer;
}

class ShaderCodeContainer : public QWidget
{
Q_OBJECT
public:
    explicit ShaderCodeContainer(ShaderLab::Shader shadertype, QWidget *parent = 0);
    void setText(const QString&);
    QString getText(void);
    ShaderLab::Shader getShaderType(void);

private:
    Ui::ShaderCodeContainer *ui;
    ShaderLab::Shader shaderType;

};

#endif // SHADERCODECONTAINER_H
