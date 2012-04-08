#ifndef GLOBALPROPERTIES_H
#define GLOBALPROPERTIES_H

#include <QDockWidget>
#include "MainWindow.h"

#include "qtpropertymanager.h"
#include "qtvariantproperty.h"
#include "qttreepropertybrowser.h"

#include <QtVariantProperty>
#include <QtTreePropertyBrowser>

namespace Ui {
    class GlobalProperties;
}

class GlobalProperties : public QDockWidget
{
    Q_OBJECT

public:
    explicit GlobalProperties(MainWindow *parent = 0);
    ~GlobalProperties();

private:
    Ui::GlobalProperties *ui;
};

#endif // GLOBALPROPERTIES_H
