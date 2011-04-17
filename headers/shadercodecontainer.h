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
    ~ShaderCodeContainer();
    void setText(const QString&);
    QString getText(void);
    ShaderLab::Shader getShaderType(void);

private:
    Ui::ShaderCodeContainer *ui;
    ShaderLab::Shader shaderType;

private slots:
    void textChanged(void);

signals:
    void textChanged(ShaderLab::Shader);

};

#endif // SHADERCODECONTAINER_H
