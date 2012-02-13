#include "SLTabWidget.h"
#include <QToolTip>
#include <QMouseEvent>
#include <QDebug>
#include "SLTabBar.h"
#include "ShaderCodeContainer.h"
#include "SLCodeContainer.h"


SLTabWidget::SLTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    next = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Tab), this);


    SLTabBar *tabBar = new SLTabBar();
    setTabBar(tabBar);

    connect(this, SIGNAL(tabCloseRequested(int)),
            this, SLOT(tabCloseRequested(int)));

    connect(tabBar, SIGNAL(signal_TabClicked()),
            this, SLOT(changeActivationStatus()));

    connect(next, SIGNAL(activated()),
            this, SLOT(nextTab()));
}

SLTabWidget::~SLTabWidget()
{
    delete next;
}

void SLTabWidget::tabCloseRequested(int index)
{
    ShaderCodeContainer* scc = (ShaderCodeContainer*)widget(index);
    //cc->closeRequest();
    emit tabCloseRequested(scc->getShaderType());
}

void SLTabWidget::changeActivationStatus()
{
    ShaderCodeContainer* ssc= (ShaderCodeContainer*)currentWidget();
    //ssc->changeActivatedStatus();
    emit changeActivationStatus(ssc->getShaderType());
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
}

void SLTabWidget::setFocus()
{
    ShaderCodeContainer * scc = (ShaderCodeContainer *)currentWidget();
    if(scc != NULL)
        scc->setFocus();
}

void SLTabWidget::closeTab(QWidget* w)
{
    removeTab(indexOf(w));
    if(count() == 0)
        setVisible(false);
}

int SLTabWidget::addTab ( QWidget * page, const QIcon & icon, const QString & label )
{
    setVisible(true);
    ShaderCodeContainer *cc = (ShaderCodeContainer *)page;
    connect(cc, SIGNAL(closeSignal(QWidget*)), this, SLOT(closeTab(QWidget*)));
    return QTabWidget::addTab(page, icon, label);
}
