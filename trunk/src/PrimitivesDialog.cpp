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

int PrimitivesDialog::getCurrentOutputPrimitiveIndex(void)
{
    return ui->comboBoxOutput->currentIndex();
}


GLenum PrimitivesDialog::getCurrentOutputPrimitive(void)
{

    return ui->comboBoxOutput->itemData(ui->comboBoxOutput->currentIndex()).value<int>();
}

GLuint PrimitivesDialog::getOutputVertexCount(void)
{
    return ui->vertexCount->value();
}

void PrimitivesDialog::primitiveSetup()
{
    ui->comboBoxOutput->addItem("GL_POINTS", GL_POINTS);
    ui->comboBoxOutput->addItem("GL_LINE_STRIP", GL_LINE_STRIP);
    ui->comboBoxOutput->addItem("GL_TRIANGLE_STRIP", GL_TRIANGLE_STRIP);
    ui->comboBoxOutput->setCurrentIndex(2); //GL_TRIANGLE_STRIP
}

void PrimitivesDialog::setMaxOutputVertexOut(int value)
{
    ui->vertexCount->setMaximum(value);
}
