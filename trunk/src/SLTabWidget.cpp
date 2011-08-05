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


//SLTabBar* SLTabWidget::getTabBar(void)
//{
//    return (SLTabBar*) this->tabBar();
//}

void SLTabWidget::tabCloseRequested(int index)
{
    qDebug() << ShaderLab::shaderToStr(((ShaderCodeContainer*)widget(index))->getShaderType());
    emit tabCloseRequested(((ShaderCodeContainer*)widget(index))->getShaderType());
}

void SLTabWidget::changeActivationStatus()
{
    emit changeActivationStatus(((ShaderCodeContainer*)currentWidget())->getShaderType());
}
