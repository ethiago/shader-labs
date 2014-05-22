#include "GlobalProperties.h"
#include "ui_GlobalProperties.h"

GlobalProperties::GlobalProperties(MainWindow *mw) :
    QDockWidget(NULL),
    ui(new Ui::GlobalProperties)
{
    ui->setupUi(this);

    QAction *act = toggleViewAction();
    act->setText("Global properties");
    act->setShortcut(QKeySequence::fromString("Ctrl+G"));
    mw->menuViewInsertAction(act);
}

GlobalProperties::~GlobalProperties()
{
    delete ui;
}
