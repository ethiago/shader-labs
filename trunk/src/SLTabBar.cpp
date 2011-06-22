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
        //QToolTip::showText(e->pos(),"SLTabBar Clicou em: " + QString::number(this->tabAt(e->pos())));
        emit signal_TabClicked();
}


