#include <QFont>

#include "shadercodecontainer.h"
#include "ui_shadercodecontainer.h"


ShaderCodeContainer::ShaderCodeContainer(ShaderLab::Shader shadertype, QWidget *parent) :
    QWidget(parent), ui(new Ui::ShaderCodeContainer)
{
    ui->setupUi(this);
    ui->shaderCodeBox->setFont(QFont("Courier"));
    shaderType = shadertype;

    ui->shaderCodeBox->setFont(QFont("Courier"));
}

ShaderCodeContainer::~ShaderCodeContainer()
{
    delete ui;
}

void ShaderCodeContainer::setText(const QString& code)
{
    ui->shaderCodeBox->setText(code);
}

QString ShaderCodeContainer::getText()
{
    return ui->shaderCodeBox->toPlainText();
}

ShaderLab::Shader ShaderCodeContainer::getShaderType(void)
{
    return shaderType;
}
