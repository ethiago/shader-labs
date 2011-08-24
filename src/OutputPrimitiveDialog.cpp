#include "headers/OutputPrimitiveDialog.h"
#include "ui_OutputPrimitiveDialog.h"

OutputPrimitiveDialog::OutputPrimitiveDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::OutputPrimitiveDialog)
{
    ui->setupUi(this);
}

OutputPrimitiveDialog::~OutputPrimitiveDialog()
{
    delete ui;
}

void OutputPrimitiveDialog::changeEvent(QEvent *e)
{
    QDialog::changeEvent(e);
    switch (e->type()) {
    case QEvent::LanguageChange:
        ui->retranslateUi(this);
        break;
    default:
        break;
    }
}
