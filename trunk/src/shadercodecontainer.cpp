#include <QFont>
#include <QMouseEvent>
#include "shadercodecontainer.h"
#include "ui_shadercodecontainer.h"


ShaderCodeContainer::ShaderCodeContainer(ShaderLab::Shader shadertype, QWidget *parent) :
    QWidget(parent), ui(new Ui::ShaderCodeContainer)
{
    ui->setupUi(this);
    ui->shaderCodeBox->setFont(QFont("Courier"));
    ui->shaderCodeBox->setAcceptRichText(false);

    shaderType = shadertype;

    connect(ui->shaderCodeBox, SIGNAL(textChanged()), this, SLOT(textChanged()));
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

void ShaderCodeContainer::textChanged(void)
{
    emit textChanged(shaderType);
}

void ShaderCodeContainer::mouseReleaseEvent(QMouseEvent *e)
{
    emit clicked(e->button(), shaderType);
}

void ShaderCodeContainer::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
        emit doubleClicked();
}

void ShaderCodeContainer::setActivatedCode(bool active)
{
    ui->shaderCodeBox->setEnabled(active);
}
