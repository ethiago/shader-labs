#include <QFile>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QSize>

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "GLDisplay.h"
#include "SLTabWidget.h"


/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* +++++++++++++++++ Constructors and destructors ++++++++++++++++++ */

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    find = new Find(this);
    find->setVisibility(false);
    find->setOkVisible(false);
    ui->centralLayout->addWidget(find);


    {
    QAction* act = find->toggleViewAction();
    act->setText("Find/Replace");
    act->setIcon(QIcon(":/ico/search.png"));
    act->setShortcut(QKeySequence::fromString("Ctrl+F"));
    ui->menuView->insertAction(0, act);
    }

    {
    QAction* act = ui->dockOutPutWidget->toggleViewAction();
    act->setText("Application output");
    act->setIcon(QIcon(":/ico/applicationOutputIcon"));
    ui->menuView->insertAction(0,act);
    }

    {
    QAction* act = ui->dockRenderWidget->toggleViewAction();
    act->setText("Render area");
    act->setIcon(QIcon(":/ico/renderAreaIcon"));
    ui->menuView->insertAction(0,act);
    }

    ui->dockOutPutWidget->setVisible(true);

    connect(ui->actionExit, SIGNAL(triggered()),
            this, SLOT(exitApplication()));

    connect(ui->actionRunAll, SIGNAL(triggered()),
            this, SIGNAL(runShaders()));

    connect(ui->actionSaveAll, SIGNAL(triggered()),
            this, SIGNAL(saveAll()));

    connect(ui->actionWireframe, SIGNAL(triggered()),
            this, SIGNAL(wireframeClicked()));

    connect(ui->saveImageButton, SIGNAL(clicked()),
            this, SIGNAL(saveResultAsImage()));

    connect(ui->actionNewFile, SIGNAL(triggered()),
            this, SIGNAL(newShaderActionClicked()));

    connect(ui->actionOpenCode, SIGNAL(triggered()),
            this, SIGNAL(openShaderActionClicked()));

    connect(ui->ckbLightRotation, SIGNAL(toggled(bool)),
            this, SIGNAL(lightRotationToggle(bool)));

    connect(ui->actionLoad_Project, SIGNAL(triggered()),
            this, SIGNAL(loadProject()));

    connect(ui->actionSave_as_project, SIGNAL(triggered()),
            this, SIGNAL(saveAsProject()));

    connect(ui->actionSave_project, SIGNAL(triggered()),
            this, SIGNAL(saveProject()));

    connect(ui->actionChange_output_primitive, SIGNAL(triggered()),
            this, SIGNAL(newPrimitiveDialog()));

    connect(ui->actionSaveFile, SIGNAL(triggered()),
            this, SIGNAL(saveShader()));

    connect(ui->actionSaveAs, SIGNAL(triggered()),
            this, SIGNAL(saveShaderAs()));

    connect(ui->actionNew_Object, SIGNAL(triggered()),
            this, SIGNAL(newObject()));

    connect(ui->originButton, SIGNAL(toggled(bool)),
            this, SIGNAL(origin(bool)));

    connect(ui->objectsVisibility, SIGNAL(toggled(bool)),
            this, SIGNAL(objectsVisibility(bool)));

}

MainWindow::~MainWindow()
{
    delete ui;
    delete tabArea;
}


/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* ++++++++++++++++++++++ Getters and setters ++++++++++++++++++++++ */

/* Setter for the output text on the screen. */
bool MainWindow::setOutputText(const QString& s)
{
    ui->outputTextBox->clear();
    ui->outputTextBox->setPlainText(s);

    return true;
}

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* +++++++++++++++++++++++++++++ Slots +++++++++++++++++++++++++++++ */

/* Associated with the '' signal. */
/* */
void MainWindow::exitApplication(void)
{
    close(); //closeEvent
}


/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* +++++++++++++++++++++++++ Other methods +++++++++++++++++++++++++ */

/* */
void MainWindow::closeEvent(QCloseEvent *event)
{
    emit programClose(event);
}

void MainWindow::setGLDisplay(GLDisplay * display)
{
    ui->verticalLayout->addWidget(display);
}


QMenu* MainWindow::modelsMenu(void)
{
    return ui->menu_Models;
}

void MainWindow::menuViewInsertAction( QAction* act)
{
    ui->menuView->insertAction(0, act);
}

void MainWindow::menuViewRemoveAction(QAction* act)
{
    ui->menuView->removeAction(act);
}

QAction* MainWindow::menuChangeOutputPrimitive(void)
{
    return ui->actionChange_output_primitive;
}

void MainWindow::setSecondTitle(const QString& title)
{
    if(title.isEmpty())
        setWindowTitle("ShaderLabs");
    else
        setWindowTitle("ShaderLabs - " + title);
}

void MainWindow::setEnableMenuGeometryShader(bool v)
{
    ui->menu_Geometry_Shader->setEnabled(v);
}

SLTabWidget* MainWindow::createTabWidget()
{
    tabArea = new SLTabWidget(this);
    tabArea->setTabsClosable(true);
    tabArea->setMovable(true);
    tabArea->setVisible(false);
    ui->centralLayout->addWidget(tabArea);

    connect(find, SIGNAL(findNext(QString)),
            tabArea, SLOT(findNext(QString)));

    connect(find, SIGNAL(findBack(QString)),
            tabArea, SLOT(findBack(QString)));

    connect(find, SIGNAL(replaceNext(QString,QString)),
            tabArea, SLOT(replaceNext(QString,QString)));

    connect(find, SIGNAL(replaceAll(QString,QString)),
            tabArea, SLOT(replaceAll(QString,QString)));

    connect(tabArea, SIGNAL(changeVisibility(bool)),
            this, SLOT(tabVisibilityChanged(bool)));

    return tabArea;
}

void MainWindow::shaderLog(const QString& log)
{
    ui->outputTextBox->clear();
    ui->outputTextBox->setPlainText(log);
}

void MainWindow::tabVisibilityChanged(bool v)
{
    find->setOkVisible(v);
}
