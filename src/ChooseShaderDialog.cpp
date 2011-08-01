#include "ChooseShaderDialog.h"
#include "ui_ChooseShaderDialog.h"

ChooseShaderDialog::ChooseShaderDialog(QWidget *parent) :
    QDialog(parent, Qt::WindowTitleHint | Qt::WindowSystemMenuHint),
    ui(new Ui::Dialog)
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
            this, SLOT(chosenShader(ShaderLab::Shader)));

}

ShaderLab::Shader ChooseShaderDialog::lastChosenShader()
{
    return m_lastChosenShader;
}

void ChooseShaderDialog::chosenShader(ShaderLab::Shader shadertype)
{
    this->accept();
    m_lastChosenShader = shadertype;
}
