#include "codechoicedialog.h"
#include "ui_codechoicedialog.h"

CodeChoiceDialog::CodeChoiceDialog(QWidget *parent) :
        QDialog(parent), ui(new Ui::Dialog)
{
    ui->setupUi(this);

    connect(ui->openVertex, SIGNAL(clicked()), this, SLOT(openVertex()));
    connect(ui->openFragment, SIGNAL(clicked()), this, SLOT(openFragment()));
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
    emit shader(shadertype);
}
