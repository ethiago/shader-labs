#include "vertexproperties.h"
#include "ui_GlobalProperties.h"
#include "MainWindow.h"

VertexProperties::VertexProperties(MainWindow *mw) :
    QDockWidget(NULL),
    ui(new Ui::GlobalProperties)
{
    ui->setupUi(this);

    QAction *act = toggleViewAction();
    act->setText("Model properties");
    act->setShortcut(QKeySequence::fromString("Ctrl+M"));
    mw->menuViewInsertAction(act);

    container = new QWidget();
    verticalLayout = new QVBoxLayout(container);

    setWidget(container);
    combo = new QComboBox();
    verticalLayout->addWidget(combo);

    setWindowTitle("Model Properties");

    connect(combo, SIGNAL(currentIndexChanged(int)),
            SIGNAL(objectChanged(int)) );

}

int VertexProperties::addItem()
{
    int size = combo->count();
    combo->addItem("Model - " + QString::number(size+1));
    combo->setCurrentIndex(size);
    return size;
}

VertexProperties::~VertexProperties()
{
    delete ui;
}

void VertexProperties::addWidget(QWidget * widget)
{
    verticalLayout->addWidget(widget);
}
