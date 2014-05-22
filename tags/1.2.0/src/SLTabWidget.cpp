#include "SLTabWidget.h"
#include <QToolTip>
#include <QMouseEvent>
#include <QDebug>
#include "SLTabBar.h"
#include "slCodeContainer2.h"


SLTabWidget::SLTabWidget(QWidget *parent) :
    QTabWidget(parent)
{
    next = new QShortcut(QKeySequence(Qt::CTRL + Qt::Key_Tab), this);


    SLTabBar *tabBar = new SLTabBar();
    setTabBar(tabBar);

    connect(tabBar, SIGNAL(doubleClicked()),
            this, SLOT(changeActivationStatus()));

    connect(next, SIGNAL(activated()),
            this, SLOT(nextTab()));
}

SLTabWidget::~SLTabWidget()
{
    delete next;
}

void SLTabWidget::changeActivationStatus()
{
    SLCodeContainer2* cc = (SLCodeContainer2*)currentWidget();
    cc->changeActivatedStatus();
}

void SLTabWidget::findNext(const QString& s)
{
    SLCodeContainer2 * cc = (SLCodeContainer2 *)currentWidget();
    if(cc != NULL)
        cc->findNext(s);
}

void SLTabWidget::findBack(const QString& s)
{
    SLCodeContainer2 * cc = (SLCodeContainer2 *)currentWidget();
    if(cc != NULL)
        cc->findBack(s);
}

void SLTabWidget::replaceNext(const QString& s, const QString& r)
{
    SLCodeContainer2 * cc = (SLCodeContainer2 *)currentWidget();
    if(cc != NULL)
        cc->replaceNext(s,r);
}

void SLTabWidget::replaceAll(const QString& s, const QString& r)
{
    SLCodeContainer2 * cc = (SLCodeContainer2 *)currentWidget();
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
    SLCodeContainer2 * cc = (SLCodeContainer2 *)currentWidget();
    if(cc != NULL)
        cc->setFocus();
}

void SLTabWidget::closeTab(QWidget* w)
{
    int idx = -1;
    idx = indexOf(w);
    if(idx < 0)
        return;
    removeTab(idx);
    if(count() == 0)
    {
        setVisible(false);
        emit changeVisibility(false);
    }
}

int SLTabWidget::addTab ( SLCodeContainer2 * container, const QIcon & icon, const QString & label )
{
    if(count() == 0)
    {
        setVisible(true);
        emit changeVisibility(true);
    }
    return QTabWidget::addTab(container, icon, label);
}

void SLTabWidget::setTabTitle(const QString& title, QWidget*w)
{
    setTabText(indexOf(w), title);
}

void SLTabWidget::setTabIcon(const QIcon& icon, QWidget*w)
{
    QTabWidget::setTabIcon(indexOf(w),icon);
}
