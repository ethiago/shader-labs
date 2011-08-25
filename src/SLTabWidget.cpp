#include "SLTabWidget.h"
#include <QToolTip>
#include <QMouseEvent>
#include <QDebug>
#include "SLTabBar.h"
#include "ShaderCodeContainer.h"


SLTabWidget::SLTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    next = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_PageDown), this);
    previous = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_PageUp), this);

    SLTabBar *tabBar = new SLTabBar();
    setTabBar(tabBar);

    connect(this, SIGNAL(tabCloseRequested(int)),
            this, SLOT(tabCloseRequested(int)));

    connect(tabBar, SIGNAL(signal_TabClicked()),
            this, SLOT(changeActivationStatus()));

    connect(next, SIGNAL(activated()),
            this, SLOT(nextTab()));

    connect(previous, SIGNAL(activated()),
            this, SLOT(previousTab()));
}

SLTabWidget::~SLTabWidget()
{
    delete next;
    delete previous;
}

void SLTabWidget::tabCloseRequested(int index)
{
    qDebug() << ShaderLab::shaderToStr(((ShaderCodeContainer*)widget(index))->getShaderType());
    emit tabCloseRequested(((ShaderCodeContainer*)widget(index))->getShaderType());
}

void SLTabWidget::changeActivationStatus()
{
    emit changeActivationStatus(((ShaderCodeContainer*)currentWidget())->getShaderType());
}

void SLTabWidget::findNext(const QString& s)
{
    ShaderCodeContainer * scc = (ShaderCodeContainer *)currentWidget();
    if(scc != NULL)
        scc->findNext(s);
}

void SLTabWidget::findBack(const QString& s)
{
    ShaderCodeContainer * scc = (ShaderCodeContainer *)currentWidget();
    if(scc != NULL)
        scc->findBack(s);
}

void SLTabWidget::replaceNext(const QString& s, const QString& r)
{
    ShaderCodeContainer * scc = (ShaderCodeContainer *)currentWidget();
    if(scc != NULL)
        scc->replaceNext(s,r);
}

void SLTabWidget::replaceAll(const QString& s, const QString& r)
{
    ShaderCodeContainer * scc = (ShaderCodeContainer *)currentWidget();
    if(scc != NULL)
        scc->replaceAll(s,r);
}
void SLTabWidget::previousTab(void)
{
    if(currentIndex() == 0 && count() > 0)
        setCurrentIndex(count() - 1);
    else if(count() > 0)
        setCurrentIndex(currentIndex() - 1);
}

void SLTabWidget::nextTab(void)
{
    int newCurrent = (currentIndex() + 1) % count();
    if(count() > 0)
        setCurrentIndex(newCurrent);

    qDebug() << newCurrent;
}
