#ifndef FIND_H
#define FIND_H

#include <QWidget>

namespace Ui{
    class Find;
}

class Find : public QWidget
{
    Q_OBJECT

    Ui::Find *ui;
    QAction* m_toggleViewAction;
    bool okVisible;

public:
    explicit Find(QWidget *parent = 0);

    QAction* toggleViewAction(void);

    void setVisibility(bool);

    void setOkVisible(bool);

signals:
    void findNext(const QString&);
    void findBack(const QString&);
    void replaceNext(const QString&, const QString&);
    void replaceAll(const QString&, const QString&);

public slots:
    void toggle(bool);
    void findNext();
    void findBack();
    void replaceNext();
    void replaceAll();
    void closeButton();



};

#endif // FIND_H
