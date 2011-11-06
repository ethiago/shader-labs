#ifndef SLTABWIDGET_H
#define SLTABWIDGET_H

#include <QTabWidget>
#include <QShortcut>
#include "Global.h"

class SLTabBar;

class SLTabWidget : public QTabWidget
{
    Q_OBJECT

    QShortcut * next;

public:
    explicit SLTabWidget(QWidget *parent = 0);
    ~SLTabWidget();

signals:
    void tabCloseRequested(ShaderLab::Shader);
    void changeActivationStatus(ShaderLab::Shader);

public slots:
    void tabCloseRequested(int index);
    void changeActivationStatus();

    void findNext(const QString&);
    void findBack(const QString&);
    void replaceNext(const QString&, const QString&);
    void replaceAll(const QString&, const QString&);

    void previousTab(void);
    void nextTab(void);

    void setFocus();

};

#endif // SLTABWIDGET_H
