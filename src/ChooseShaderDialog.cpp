#include "ChooseShaderDialog.h"
#include "ui_ChooseShaderDialog.h"

ChooseShaderDialog::ChooseShaderDialog(QWidget *parent) : QDialog(parent), ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

ChooseShaderDialog::~ChooseShaderDialog()
{
    QMap<ShaderLab::Shader, SLCommandLinkButton*>::iterator it;
    SLCommandLinkButton* pt;

    for(it = buttons.begin(); it != buttons.end(); ++it)
    {
        pt = it.value();
        delete pt;
    }

    delete ui;
}

void ChooseShaderDialog::addButton(ShaderLab::Shader shadertype)
{
    SLCommandLinkButton *button = new SLCommandLinkButton(shadertype, ui->scrollAreaWidgetContents);
    buttons.insert(shadertype, button);

    ui->verticalLayout->addWidget(button);

    connect(button, SIGNAL(clicked(ShaderLab::Shader)),
            this, SLOT(arebaba(ShaderLab::Shader)));

}

void ChooseShaderDialog::arebaba(ShaderLab::Shader shadertype)
{
    this->close();
    emit shader(shadertype);
}
