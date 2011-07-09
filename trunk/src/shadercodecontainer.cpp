#include <QFont>
#include <QMouseEvent>
#include "shadercodecontainer.h"
#include "ui_shadercodecontainer.h"



ShaderCodeContainer::ShaderCodeContainer(ShaderLab::Shader shadertype, QWidget *parent) :
    QWidget(parent), ui(new Ui::ShaderCodeContainer)
{
    ui->setupUi(this);
    ui->shaderCodeBox->setFont(QFont("Courier"));
    ui->shaderCodeBox->setAcceptRichText(false);

    activePalette = ui->shaderCodeBox->palette();
    inactivePalette = ui->shaderCodeBox->palette();

    shaderType = shadertype;

    QBrush brush(QColor(128, 128, 128, 255));
    brush.setStyle(Qt::SolidPattern);
    inactivePalette.setBrush(QPalette::Active, QPalette::Text, brush);
    inactivePalette.setBrush(QPalette::Inactive, QPalette::Text, brush);
    QBrush brush1(QColor(159, 158, 158, 255));
    brush1.setStyle(Qt::SolidPattern);
    inactivePalette.setBrush(QPalette::Disabled, QPalette::Text, brush1);

    ui->shaderCodeBox->setTabStopWidth(7*4);

    connect(ui->shaderCodeBox, SIGNAL(textChanged()), this, SLOT(textChanged()));

    connect(ui->shaderCodeBox, SIGNAL(cursorPositionChanged()), this, SLOT(timeout()));
}

ShaderCodeContainer::~ShaderCodeContainer()
{
    delete ui;
}

void ShaderCodeContainer::setText(const QString& code)
{
    ui->shaderCodeBox->setText(code);
    setActivatedCode(true);
}

QString ShaderCodeContainer::getText()
{
    return ui->shaderCodeBox->toPlainText();
}

ShaderLab::Shader ShaderCodeContainer::getShaderType(void)
{
    return shaderType;
}

void ShaderCodeContainer::textChanged(void)
{
    emit textChanged(shaderType);
}

void ShaderCodeContainer::mouseReleaseEvent(QMouseEvent *e)
{
    emit clicked(e->button(), shaderType);
}

void ShaderCodeContainer::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
        emit doubleClicked();
}

void ShaderCodeContainer::setActivatedCode(bool active)
{
    if(active)
    {
        ui->shaderCodeBox->setPalette(activePalette);
    }else
    {
        ui->shaderCodeBox->setPalette(inactivePalette);
    }
}

void ShaderCodeContainer::timeout(void)
{
    QTextCursor tc = ui->shaderCodeBox->textCursor();
    int p = tc.position();
    ui->cursorPositionLabel->setText(QString::number(p));
}
