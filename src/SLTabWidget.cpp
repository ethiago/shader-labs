#include "SLTabWidget.h"
#include <QToolTip>
#include <QMouseEvent>
#include <QDebug>
#include "SLTabBar.h"
#include "ShaderCodeContainer.h"


SLTabWidget::SLTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    SLTabBar *tabBar = new SLTabBar();
    setTabBar(tabBar);

    connect(this, SIGNAL(tabCloseRequested(int)),
            this, SLOT(tabCloseRequested(int)));

    connect(tabBar, SIGNAL(signal_TabClicked()),
            this, SLOT(changeActivationStatus()));
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
