#include <QFont>
#include <QDebug>
#include <QMouseEvent>
#include "SLCodeContainer.h"
#include "ui_ShaderCodeContainer.h"
#include "HighLighter.h"
#include "codeeditor.h"



SLCodeContainer::SLCodeContainer(ShaderLab::Shader shadertype, const QString& title, QWidget *parent) :
    QWidget(parent), ui(new Ui::ShaderCodeContainer)
{

    m_save = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), this);
    m_active = true;
    shaderType = shadertype;

    ui->setupUi(this);
    setupCodeBox();

    activePalette = shaderCodeBox->palette();
    inactivePalette = shaderCodeBox->palette();


    QBrush brush(QColor(128, 128, 128, 255));
    brush.setStyle(Qt::SolidPattern);
    inactivePalette.setBrush(QPalette::Active, QPalette::Text, brush);
    inactivePalette.setBrush(QPalette::Inactive, QPalette::Text, brush);
    QBrush brush1(QColor(159, 158, 158, 255));
    brush1.setStyle(Qt::SolidPattern);
    inactivePalette.setBrush(QPalette::Disabled, QPalette::Text, brush1);

    connect(m_save, SIGNAL(activated()), this, SIGNAL(save()));

    setWindowTitle(title);
    setWindowIcon(QIcon(":/ico/running"));
}

SLCodeContainer::~SLCodeContainer()
{
    delete ui;
    delete shaderCodeBox;
}

void SLCodeContainer::setupCodeBox()
{
    shaderCodeBox = new CodeEditor(this);
    shaderCodeBox->setObjectName(QString::fromUtf8("shaderCodeBox"));

    QBrush brush1(QColor(159, 158, 158, 255));
    brush1.setStyle(Qt::SolidPattern);
    QBrush brush2(QColor(0, 0, 0, 255));
    brush2.setStyle(Qt::SolidPattern);

    QPalette palette1;
    palette1.setBrush(QPalette::Active, QPalette::Text, brush2);
    palette1.setBrush(QPalette::Inactive, QPalette::Text, brush2);
    palette1.setBrush(QPalette::Disabled, QPalette::Text, brush1);
    shaderCodeBox->setPalette(palette1);

    ui->verticalLayout->addWidget(shaderCodeBox);

    highLighter = new Highlighter(shaderCodeBox->document());
    shaderCodeBox->setFont(QFont("Courier"));

    connect(shaderCodeBox, SIGNAL(modificationChanged(bool)), this, SLOT(textChanged(bool)));
    connect(shaderCodeBox, SIGNAL(cursorPositionChanged()), this, SLOT(cursorPositionChanged()));
}

void SLCodeContainer::setText(const QString& code)
{
    shaderCodeBox->setPlainText(code);
}

QString SLCodeContainer::getText()
{
    return shaderCodeBox->toPlainText();
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
        shaderCodeBox->document()->setModified(false);
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

void SLCodeContainer::changeActivatedStatus()
{
    m_active = !m_active;
    if(m_active)
    {
        highLighter->setDocument(shaderCodeBox->document());
        shaderCodeBox->setPalette(activePalette);
        setTabIcon(QIcon(":/ico/running"));
    }else
    {
        highLighter->setDocument(NULL);
        shaderCodeBox->setPalette(inactivePalette);
        setTabIcon(QIcon(":/ico/stopped"));
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
    QTextCursor tc = shaderCodeBox->textCursor();
    QPoint p = getCursorPosition(shaderCodeBox->toPlainText(), tc.position());
    ui->cursorPositionLabel->setText(QString::number(p.x()) + ":" + QString::number(p.y()));
}

void SLCodeContainer::findNext(const QString& s)
{
    shaderCodeBox->find(s);
}

void SLCodeContainer::findBack(const QString& s)
{
    shaderCodeBox->find(s, QTextDocument::FindBackward);
}

void SLCodeContainer::setFocus()
{
    shaderCodeBox->setFocus();
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

void SLCodeContainer::setTabTitle(const QString& title)
{
    setWindowTitle(title);
    emit setTabTitle(title, this);
}

void SLCodeContainer::setTabIcon(const QIcon& icon)
{
    setWindowIcon(icon);
    emit setTabIcon(icon, this);
}

void SLCodeContainer::updateTabBar()
{
    emit setTabTitle(windowTitle(), this);
    emit setTabIcon(windowIcon(), this);
}

void SLCodeContainer::saveShader()
{
    emit save();
}

void SLCodeContainer::saveShaderAs()
{
    emit saveAs();
}
