#ifndef SLTABWIDGET_H
#define SLTABWIDGET_H

#include <QTabWidget>


class SLTabBar;

class SLTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit SLTabWidget(QWidget *parent = 0);
    SLTabBar* getTabBar(void);

signals:

public slots:

protected:

};

#endif // SLTABWIDGET_H
