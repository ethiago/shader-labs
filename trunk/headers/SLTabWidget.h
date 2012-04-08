#ifndef SLTABWIDGET_H
#define SLTABWIDGET_H

#include <QTabWidget>
#include <QShortcut>
#include "Global.h"
#include "EditorController.h"

class SLTabBar;

class SLTabWidget : public QTabWidget
{
    Q_OBJECT

    QShortcut * next;

public:
    explicit SLTabWidget(QWidget *parent = 0);
    ~SLTabWidget();
    //int addTab ( QWidget * page, const QIcon & icon, const QString & label );
    int addTab ( EditorController * controller, const QIcon & icon = QIcon(), const QString & label = QString());

signals:
    void changeVisibility(bool);

public slots:
    void tabCloseRequested(int index);
    void changeActivationStatus();

    void findNext(const QString&);
    void findBack(const QString&);
    void replaceNext(const QString&, const QString&);
    void replaceAll(const QString&, const QString&);
    void closeTab(QWidget*);
    void setTabTitle(const QString& title, QWidget*);
    void setTabIcon(const QIcon& icon, QWidget*);

    void nextTab(void);

    void setFocus();

};

#endif // SLTABWIDGET_H
