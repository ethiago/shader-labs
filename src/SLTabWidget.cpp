#include "SLTabWidget.h"
#include <QToolTip>
#include <QMouseEvent>
#include <QDebug>
#include "SLTabBar.h"
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
    SLCodeContainer* cc = (SLCodeContainer*)widget(index);
    cc->closeRequest();
}

void SLTabWidget::changeActivationStatus()
{
    SLCodeContainer* cc = (SLCodeContainer*)currentWidget();
    cc->changeActivatedStatus();
}

void SLTabWidget::findNext(const QString& s)
{
    SLCodeContainer * cc = (SLCodeContainer *)currentWidget();
    if(cc != NULL)
        cc->findNext(s);
}

void SLTabWidget::findBack(const QString& s)
{
    SLCodeContainer * cc = (SLCodeContainer *)currentWidget();
    if(cc != NULL)
        cc->findBack(s);
}

void SLTabWidget::replaceNext(const QString& s, const QString& r)
{
    SLCodeContainer * cc = (SLCodeContainer *)currentWidget();
    if(cc != NULL)
        cc->replaceNext(s,r);
}

void SLTabWidget::replaceAll(const QString& s, const QString& r)
{
    SLCodeContainer * cc = (SLCodeContainer *)currentWidget();
    if(cc != NULL)
        cc->replaceAll(s,r);
}

void SLTabWidget::nextTab(void)
{
    int newCurrent = (currentIndex() + 1) % count();
    if(count() > 0)
        setCurrentIndex(newCurrent);
}

void SLTabWidget::setFocus()
{
    SLCodeContainer * cc = (SLCodeContainer *)currentWidget();
    if(cc != NULL)
        cc->setFocus();
}

void SLTabWidget::closeTab(QWidget* w)
{
    removeTab(indexOf(w));
    if(count() == 0)
    {
        setVisible(false);
        emit changeVisibility(false);
    }
}

int SLTabWidget::addTab ( EditorController * controller, const QIcon & icon, const QString & label )
{
    if(count() == 0)
    {
        setVisible(true);
        emit changeVisibility(true);
    }
    SLCodeContainer *cc = (SLCodeContainer *)controller->codeContainer();
    connect(cc, SIGNAL(closeSignal(QWidget*)), this, SLOT(closeTab(QWidget*)));
    connect(cc, SIGNAL(setTabIcon(QIcon,QWidget*)), this, SLOT(setTabIcon(QIcon,QWidget*)));
    connect(cc, SIGNAL(setTabTitle(QString,QWidget*)), this, SLOT(setTabTitle(QString,QWidget*)));
    return QTabWidget::addTab(cc, icon, label);
}

void SLTabWidget::setTabTitle(const QString& title, QWidget*w)
{
    setTabText(indexOf(w), title);
}

void SLTabWidget::setTabIcon(const QIcon& icon, QWidget*w)
{
    QTabWidget::setTabIcon(indexOf(w),icon);
}
