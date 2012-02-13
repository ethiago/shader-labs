#include <QFont>
#include <QMouseEvent>
#include "SLCodeContainer.h"
#include "ui_ShaderCodeContainer.h"
#include "HighLighter.h"



SLCodeContainer::SLCodeContainer(ShaderLab::Shader shadertype, QWidget *parent) :
    QWidget(parent), ui(new Ui::ShaderCodeContainer)
{

    m_save = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_K), this);
    m_active = true;

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

    connect(m_save, SIGNAL(activatedAmbiguously()), this, SIGNAL(save()));
}

SLCodeContainer::~SLCodeContainer()
{
    delete ui;
}

void SLCodeContainer::setText(const QString& code)
{
    ui->shaderCodeBox->setPlainText(code);
    setActivatedCode(true);
}

QString SLCodeContainer::getText()
{
    return ui->shaderCodeBox->toPlainText();
}

ShaderLab::Shader SLCodeContainer::getShaderType(void)
{
    return shaderType;
}

void SLCodeContainer::textChanged(bool v)
{
    if(v)
    {
        emit textChanged(shaderType);
        emit textChanged();
        ui->shaderCodeBox->document()->setModified(false);
    }
}

void SLCodeContainer::mouseReleaseEvent(QMouseEvent *e)
{
    emit clicked(e->button(), shaderType);
}

void SLCodeContainer::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
        emit doubleClicked(shaderType);
}

void SLCodeContainer::setActivatedCode(bool active)
{
    m_active = active;
    if(active)
    {
        highLighter->setDocument(ui->shaderCodeBox->document());
        ui->shaderCodeBox->setPalette(activePalette);
        setWindowIcon(QIcon(":/ico/running"));
    }else
    {
        highLighter->setDocument(NULL);
        ui->shaderCodeBox->setPalette(inactivePalette);
        setWindowIcon(QIcon(":/ico/stopped"));
    }
}

void SLCodeContainer::changeActivatedStatus()
{
    m_active = !m_active;
    if(m_active)
    {
        highLighter->setDocument(ui->shaderCodeBox->document());
        ui->shaderCodeBox->setPalette(activePalette);
        setWindowIcon(QIcon(":/ico/running"));
    }else
    {
        highLighter->setDocument(NULL);
        ui->shaderCodeBox->setPalette(inactivePalette);
        setWindowIcon(QIcon(":/ico/stopped"));
    }
    emit activateStatusChanged();
}

void SLCodeContainer::closeRequest()
{
    emit closeRequestSignal();
}

QPoint SLCodeContainer::getCursorPosition(const QString& text, int pos)
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

void SLCodeContainer::cursorPositionChanged(void)
{
    QTextCursor tc = ui->shaderCodeBox->textCursor();
    QPoint p = getCursorPosition(ui->shaderCodeBox->toPlainText(), tc.position());
    ui->cursorPositionLabel->setText(QString::number(p.x()) + ":" + QString::number(p.y()));
}

void SLCodeContainer::findNext(const QString& s)
{
    ui->shaderCodeBox->find(s);
}

void SLCodeContainer::findBack(const QString& s)
{
    ui->shaderCodeBox->find(s, QTextDocument::FindBackward);
}

void SLCodeContainer::setFocus()
{
    ui->shaderCodeBox->setFocus();
}

void SLCodeContainer::replaceNext(const QString&, const QString&)
{
}

void SLCodeContainer::replaceAll(const QString&, const QString&)
{

}

void SLCodeContainer::close()
{
    emit closeSignal(this);
}

bool SLCodeContainer::activateCode()
{
    return m_active;
}
