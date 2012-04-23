#include "SLTexture3dDialog.h"
#include "ui_SLTexture3dDialog.h"

SLTexture3DDialog::SLTexture3DDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::SLTexture3DDialog)
{
    ui->setupUi(this);

    ui->comboType->addItem("UNSIGNED INT 8");
    ui->comboType->addItem("UNSIGNED INT 16");
}

SLTexture3DDialog::~SLTexture3DDialog()
{
    delete ui;
}

void SLTexture3DDialog::setFileName(const QString& fn)
{
    ui->lFileName->setText(fn);
    ui->sWidth->setValue(256);
    ui->sHeight->setValue(256);
    ui->sDepth->setValue(256);
    ui->comboType->setCurrentIndex(0);
}

int SLTexture3DDialog::getFileWidth()
{
    return ui->sWidth->value();
}

int SLTexture3DDialog::getFileHeight()
{
    return ui->sHeight->value();
}

int SLTexture3DDialog::getFileDepth()
{
    return ui->sDepth->value();
}

int SLTexture3DDialog::getLengthData()
{
    switch(ui->comboType->currentIndex())
    {
        case 0:
            return 1;
        default:
            return 2;
    }
}
