#include "PrimitivesDialog.h"
#include "ui_PrimitivesDialog.h"



PrimitivesDialog::PrimitivesDialog() :
    QDialog(NULL),
    ui(new Ui::PrimitivesDialog)
{
    ui->setupUi(this);

    m_toogleView = new QAction(QString("Primitives Dialog"), NULL);
    m_toogleView->setCheckable(true);
    m_toogleView->setChecked(false);

    connect(m_toogleView, SIGNAL(triggered(bool)),
            this, SLOT(viewAction(bool)));

    primitiveSetup();
}

PrimitivesDialog::~PrimitivesDialog()
{
    delete ui;
}

QAction * PrimitivesDialog::toogleViewAction()
{
    return m_toogleView;
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

void PrimitivesDialog::viewAction(bool v)
{
    if(v)
        show();
    else
        hide();
}

void PrimitivesDialog::showEvent(QShowEvent *e)
{
    m_toogleView->setChecked(true);
}

void PrimitivesDialog::hideEvent(QHideEvent *e)
{
    m_toogleView->setChecked(false);
}
