#ifndef SLTABWIDGET_H
#define SLTABWIDGET_H

#include <QTabWidget>
#include "Global.h"

class SLTabBar;

class SLTabWidget : public QTabWidget
{
    Q_OBJECT

public:
    explicit SLTabWidget(QWidget *parent = 0);
    //SLTabBar* getTabBar(void);

signals:
    void tabCloseRequested(ShaderLab::Shader);
    void changeActivationStatus(ShaderLab::Shader);

public slots:
    void tabCloseRequested(int index);
    void changeActivationStatus();

};

#endif // SLTABWIDGET_H
