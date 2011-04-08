#include "codechoicedialog.h"
#include "ui_codechoicedialog.h"

CodeChoiceDialog::CodeChoiceDialog(QWidget *parent) :
        QDialog(parent), ui(new Ui::Dialog)
{
    ui->setupUi(this);
}

/*void CodeChoiceDialog::openVertex()
{
    emit shader(ShaderLab::Vertex);
    close();
}

void CodeChoiceDialog::openFragment()
{
    emit shader(ShaderLab::Fragment);
    close();
}*/

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
