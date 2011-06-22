#ifndef SLTABBAR_H
#define SLTABBAR_H

#include <QTabBar>

class SLTabBar : public QTabBar
{
    Q_OBJECT
public:
    explicit SLTabBar(QWidget *parent = 0);

signals:
    void signal_TabClicked();

public slots:

protected:
    void mouseDoubleClickEvent(QMouseEvent *);

};

#endif // SLTABBAR_H
