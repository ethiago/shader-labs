#include "PrimitivesDialog.h"
#include "ui_PrimitivesDialog.h"

PrimitivesDialog::PrimitivesDialog(QStringList ouTexts, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrimitivesDialog)
{
    ui->setupUi(this);

    ui->comboBoxOutput->addItems(ouTexts);
    ui->comboBoxOutput->setCurrentIndex(2); //GL_TRIANGLES - To change, check RenderController::primitives
}

PrimitivesDialog::~PrimitivesDialog()
{
    delete ui;
}

void PrimitivesDialog::changeCurrentInputType(const QString& s)
{
    ui->comboBoxInput->clear();
    ui->comboBoxInput->addItem(s);
}

void PrimitivesDialog::changeEvent(QEvent *e)
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

int PrimitivesDialog::getCurrentOutputPrimitiveIndex(void)
{
    return ui->comboBoxOutput->currentIndex();
}

int PrimitivesDialog::getCurrentInputPrimitiveIndex(void)
{
    return ui->comboBoxInput->currentIndex();
}
