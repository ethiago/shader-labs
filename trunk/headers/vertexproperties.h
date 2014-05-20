#ifndef VERTEXPROPERTIES_H
#define VERTEXPROPERTIES_H

#include <QDockWidget>
#include <QVBoxLayout>
#include <QComboBox>

namespace Ui {
    class GlobalProperties;
}

class MainWindow;

class VertexProperties : public QDockWidget
{
    Q_OBJECT

    QVBoxLayout * verticalLayout;
    QWidget * container;
    QComboBox * combo;

public:
    explicit VertexProperties(MainWindow *mw);
    ~VertexProperties();

    void addWidget(QWidget * widget);

    int addItem();
    
private:
    Ui::GlobalProperties *ui;

signals:
    void objectChanged(int);
    
};

#endif // VERTEXPROPERTIES_H
