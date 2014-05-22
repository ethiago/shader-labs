#include "slCodeContainer2.h"
#include "ui_ShaderCodeContainer2.h"
#include "HighLighter.h"
#include "codeeditor.h"
#include "SLTabWidget.h"

#include <QFont>
#include <QDebug>
#include <QMouseEvent>

SLCodeContainer2::SLCodeContainer2(ShaderLab::Shader shadertype, SLTabWidget *parent, const QString& title) :
    QWidget(parent), ui(new Ui::ShaderCodeContainer2)
{

    m_disableNextEvent = false;
    e_parent = parent;
    m_save = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_S), this);
    m_active = true;
    m_shaderType = shadertype;

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

    connect(m_save, SIGNAL(activated()), this, SLOT(saveFromShotcut()));

    setWindowTitle(title);
    setWindowIcon(QIcon(":/ico/running"));
}

SLCodeContainer2::~SLCodeContainer2()
{
    delete ui;
    delete shaderCodeBox;
}

void SLCodeContainer2::setupCodeBox()
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

    shaderCodeBox->increaseFont();
    shaderCodeBox->increaseFont();
    shaderCodeBox->increaseFont();
    shaderCodeBox->increaseFont();
}

void SLCodeContainer2::setText(const QString& code, bool disableNextEvent)
{
    m_disableNextEvent = disableNextEvent;
    shaderCodeBox->setPlainText(code);
}

QString SLCodeContainer2::getText()
{
    return shaderCodeBox->toPlainText();
}

ShaderLab::Shader SLCodeContainer2::shaderType(void)
{
    return m_shaderType;
}

void SLCodeContainer2::textChanged(bool v)
{
    if(v && !m_disableNextEvent)
    {
        emit textChanged(m_shaderType);
        shaderCodeBox->document()->setModified(false);
    }

    m_disableNextEvent = false;
}

void SLCodeContainer2::setActive(bool v)
{
    m_active = v;
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
}

void SLCodeContainer2::changeActivatedStatus()
{
    m_active = !m_active;
    if(m_active)
    {
        highLighter->setDocument(shaderCodeBox->document());
        shaderCodeBox->setPalette(activePalette);
        setTabIcon(QIcon(":/ico/running"));
        emit activateStatusChanged(m_shaderType, true);
    }else
    {
        highLighter->setDocument(NULL);
        shaderCodeBox->setPalette(inactivePalette);
        setTabIcon(QIcon(":/ico/stopped"));
        emit activateStatusChanged(m_shaderType, false);
    }

}

void SLCodeContainer2::closeRequest()
{
    emit closeRequestSignal(m_shaderType);
}

QPoint SLCodeContainer2::getCursorPosition(const QString& text, int pos)
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

void SLCodeContainer2::cursorPositionChanged(void)
{
    QTextCursor tc = shaderCodeBox->textCursor();
    QPoint p = getCursorPosition(shaderCodeBox->toPlainText(), tc.position());
    ui->cursorPositionLabel->setText(QString::number(p.x()) + ":" + QString::number(p.y()));
}

void SLCodeContainer2::findNext(const QString& s)
{
    shaderCodeBox->find(s);
}

void SLCodeContainer2::findBack(const QString& s)
{
    shaderCodeBox->find(s, QTextDocument::FindBackward);
}

void SLCodeContainer2::setFocus()
{
    shaderCodeBox->setFocus();
}

void SLCodeContainer2::replaceNext(const QString&, const QString&)
{
}

void SLCodeContainer2::replaceAll(const QString&, const QString&)
{

}

void SLCodeContainer2::setTabTitle(const QString& title)
{
    setWindowTitle(title);
    e_parent->setTabTitle(title,this);
}

void SLCodeContainer2::setTabIcon(const QIcon& icon)
{
    setWindowIcon(icon);
    e_parent->setTabIcon(icon,this);
}

void SLCodeContainer2::updateTabBar()
{
    e_parent->setTabIcon(windowIcon(), this);
    e_parent->setTabTitle(windowTitle(), this);
}

void SLCodeContainer2::saveFromShotcut()
{
    emit save(m_shaderType);
}

void SLCodeContainer2::hideTab()
{
    e_parent->closeTab(this);
    hide();
}

void SLCodeContainer2::showTab()
{
    e_parent->addTab(this, windowIcon(), windowTitle());
    updateTabBar();
    show();
}
