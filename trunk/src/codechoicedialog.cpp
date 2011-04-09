#include "codechoicedialog.h"
#include "ui_codechoicedialog.h"

CodeChoiceDialog::CodeChoiceDialog(QWidget *parent) :
        QDialog(parent), ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

CodeChoiceDialog::~CodeChoiceDialog()
{
    QMap<ShaderLab::Shader, CommandLinkButton*>::iterator it;
    CommandLinkButton* pt;
    for(it = buttons.begin(); it != buttons.end(); ++it)
    {
        pt = it.value();
        delete pt;
    }
    delete ui;
}

void CodeChoiceDialog::addButton(ShaderLab::Shader shadertype)
{
    CommandLinkButton *button = new CommandLinkButton(shadertype, ui->scrollAreaWidgetContents);
    buttons.insert(shadertype, button);

    ui->verticalLayout->addWidget(button);

    connect(button, SIGNAL(clicked(ShaderLab::Shader)),
            this, SLOT(arebaba(ShaderLab::Shader)));

}

void CodeChoiceDialog::arebaba(ShaderLab::Shader shadertype)
{
    this->close();
    emit shader(shadertype);
}
