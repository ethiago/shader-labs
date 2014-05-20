#include "SLTabBar.h"
#include <QToolTip>
#include <QMouseEvent>

SLTabBar::SLTabBar(QWidget *parent) :
    QTabBar(parent)
{
}

void SLTabBar::mouseDoubleClickEvent(QMouseEvent *e)
{
    if(e->button() == Qt::LeftButton)
        emit doubleClicked();
}


