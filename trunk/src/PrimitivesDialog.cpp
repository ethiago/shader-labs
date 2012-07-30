#include "PrimitivesDialog.h"
#include "ui_PrimitivesDialog.h"


PrimitivesDialog::PrimitivesDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::PrimitivesDialog)
{
    ui->setupUi(this);

    primitiveSetup();
}

PrimitivesDialog::~PrimitivesDialog()
{
    delete ui;
}

//void PrimitivesDialog::changeCurrentInputType(const QString& s)
//{
//    ui->comboBoxInput->clear();
//    ui->comboBoxInput->addItem(s);
//}

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

//int PrimitivesDialog::getCurrentInputPrimitiveIndex(void)
//{
//    return ui->comboBoxInput->currentIndex();
//}

GLenum PrimitivesDialog::getCurrentOutputPrimitive(void)
{

    return ui->comboBoxOutput->itemData(ui->comboBoxOutput->currentIndex()).value<int>();
}

//GLenum PrimitivesDialog::getCurrentInputPrimitive(void)
//{
//    return ui->comboBoxInput->itemData(ui->comboBoxInput->currentIndex()).value<int>();
//}

void PrimitivesDialog::primitiveSetup()
{
    ui->comboBoxOutput->addItem("GL_POINTS", GL_POINTS);
    ui->comboBoxOutput->addItem("GL_LINE_STRIP", GL_LINE_STRIP);
    ui->comboBoxOutput->addItem("GL_TRIANGLE_STRIP", GL_TRIANGLE_STRIP);
    ui->comboBoxOutput->setCurrentIndex(2); //GL_TRIANGLE_STRIP
}
