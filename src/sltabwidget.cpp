#include "sltabwidget.h"
#include <QToolTip>
#include <QMouseEvent>
#include <QDebug>
#include "SLTabBar.h"


SLTabWidget::SLTabWidget(QWidget *parent) :
    QTabWidget(parent)
{

    setTabBar(new SLTabBar());

}


SLTabBar* SLTabWidget::getTabBar(void)
{
    return (SLTabBar*) this->tabBar();
}

