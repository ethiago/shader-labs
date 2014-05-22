#ifndef SLTABWIDGET_H
#define SLTABWIDGET_H

#include <QTabWidget>
#include <QShortcut>
#include "Global.h"


class SLTabBar;
class SLCodeContainer2;

class SLTabWidget : public QTabWidget
{
    Q_OBJECT

    QShortcut * next;

public:
    explicit SLTabWidget(QWidget *parent = 0);
    ~SLTabWidget();
    int addTab ( SLCodeContainer2 * container, const QIcon & icon = QIcon(), const QString & label = QString());
    void closeTab(QWidget*);
    void setTabTitle(const QString& title, QWidget*);
    void setTabIcon(const QIcon& icon, QWidget*);

signals:
    void changeVisibility(bool);

public slots:
    void changeActivationStatus();

    void findNext(const QString&);
    void findBack(const QString&);
    void replaceNext(const QString&, const QString&);
    void replaceAll(const QString&, const QString&);

    void nextTab(void);

    void setFocus();

};

#endif // SLTABWIDGET_H
