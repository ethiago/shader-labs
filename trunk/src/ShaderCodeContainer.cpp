#include <QFont>
#include <QMouseEvent>
#include "ShaderCodeContainer.h"
#include "ui_ShaderCodeContainer.h"
#include "HighLighter.h"



ShaderCodeContainer::ShaderCodeContainer(ShaderLab::Shader shadertype, QWidget *parent) :
    QWidget(parent), ui(new Ui::ShaderCodeContainer)
{
    ui->setupUi(this);
    ui->shaderCodeBox->setFont(QFont("Courier"));
    //ui->shaderCodeBox->setAcceptRichText(false);
    highLighter = new Highlighter(ui->shaderCodeBox->document());

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

    connect(ui->shaderCodeBox, SIGNAL(modificationChanged(bool)), this, SLOT(textChanged(bool)));

    connect(ui->shaderCodeBox, SIGNAL(cursorPositionChanged()), this, SLOT(cursorPositionChanged()));
}

ShaderCodeContainer::~ShaderCodeContainer()
{
    delete ui;
}

void ShaderCodeContainer::setText(const QString& code)
{
    ui->shaderCodeBox->setPlainText(code);
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

void ShaderCodeContainer::textChanged(bool v)
{
    if(v)
        emit textChanged(shaderType);
}

void ShaderCodeContainer::mouseReleaseEvent(QMouseEvent *e)
{
    emit clicked(e->button(), shaderType);
}

void ShaderCodeContainer::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
        emit doubleClicked(shaderType);
}

void ShaderCodeContainer::setActivatedCode(bool active)
{
    if(active)
    {
        highLighter->setDocument(ui->shaderCodeBox->document());
        ui->shaderCodeBox->setPalette(activePalette);
    }else
    {
        highLighter->setDocument(NULL);
        ui->shaderCodeBox->setPalette(inactivePalette);
    }
}

QPoint ShaderCodeContainer::getCursorPosition(const QString& text, int pos)
{
    int lineCount = 0;
    int columnCount = 0;

    for(int i = 0; i < pos; ++i)
    {
        if(text[i] == '\n')
        {
            lineCount++;
            columnCount = 0;
        }else
            columnCount++;
    }

    return QPoint(lineCount+1, columnCount+1);
}

void ShaderCodeContainer::cursorPositionChanged(void)
{
    QTextCursor tc = ui->shaderCodeBox->textCursor();
    QPoint p = getCursorPosition(ui->shaderCodeBox->toPlainText(), tc.position());
    ui->cursorPositionLabel->setText(QString::number(p.x()) + ":" + QString::number(p.y()));
}

void ShaderCodeContainer::findNext(const QString& s)
{
    ui->shaderCodeBox->find(s);
}

void ShaderCodeContainer::findBack(const QString& s)
{
    ui->shaderCodeBox->find(s, QTextDocument::FindBackward);
}

void ShaderCodeContainer::setFocus()
{
    ui->shaderCodeBox->setFocus();
}

void ShaderCodeContainer::replaceNext(const QString&, const QString&)
{
}

void ShaderCodeContainer::replaceAll(const QString&, const QString&)
{

}
