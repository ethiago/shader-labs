#ifndef GLOBALPROPERTIES_H
#define GLOBALPROPERTIES_H

#include <QDockWidget>
#include "MainWindow.h"

namespace Ui {
    class GlobalProperties;
}

class GlobalProperties : public QDockWidget
{
    Q_OBJECT

public:
    explicit GlobalProperties(MainWindow *mw);
    ~GlobalProperties();

private:
    Ui::GlobalProperties *ui;
};

#endif // GLOBALPROPERTIES_H
