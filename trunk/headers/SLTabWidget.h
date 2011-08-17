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

};

#endif // SLTABWIDGET_H
