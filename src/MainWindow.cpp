#include <QFile>
#include <QVBoxLayout>
#include <QMessageBox>
#include <QSize>

#include "MainWindow.h"
#include "ui_MainWindow.h"
#include "GLDisplay.h"
#include "SLTabWidget.h"
#include "SLTabBar.h"


/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* +++++++++++++++++ Constructors and destructors ++++++++++++++++++ */

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow)
{
    ui->setupUi(this);

    tabArea = new SLTabWidget(this);
    tabArea->setTabsClosable(true);
    tabArea->setMovable(true);
    tabArea->setVisible(false);
    ui->centralLayout->addWidget(tabArea);


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

    connect(ui->actionSaveFile, SIGNAL(triggered()),
            this, SLOT(saveFile()));

    connect(ui->actionSaveAs, SIGNAL(triggered()),
            this, SLOT(saveFileAsDialog()));

    connect(ui->actionSaveAll, SIGNAL(triggered()),
            this, SIGNAL(saveAll()));

    connect(ui->actionWireframe, SIGNAL(toggled(bool)),
            this, SIGNAL(wireframeClicked(bool)));

    connect(ui->actionSave_Result_as_Image, SIGNAL(triggered()),
            this, SIGNAL(saveResultAsImage()));

    connect(ui->actionNewFile, SIGNAL(triggered()),
            this, SIGNAL(newShaderActionClicked()));

    connect(ui->actionOpenCode, SIGNAL(triggered()),
            this, SIGNAL(openShaderActionClicked()));

    connect(tabArea, SIGNAL(tabCloseRequested(ShaderLab::Shader)),
            this, SIGNAL(closeTabRequest(ShaderLab::Shader)));

    connect(tabArea, SIGNAL(changeActivationStatus(ShaderLab::Shader)),
            this, SIGNAL(changeActivationStatusClicked(ShaderLab::Shader)));

    connect(find, SIGNAL(findNext(QString)),
            tabArea, SLOT(findNext(QString)));

    connect(find, SIGNAL(findBack(QString)),
            tabArea, SLOT(findBack(QString)));

    connect(find, SIGNAL(replaceNext(QString,QString)),
            tabArea, SLOT(replaceNext(QString,QString)));

    connect(find, SIGNAL(replaceAll(QString,QString)),
            tabArea, SLOT(replaceAll(QString,QString)));

}

MainWindow::~MainWindow()
{
    QMap<ShaderLab::Shader, ShaderCodeContainer*>::iterator it;
    ShaderCodeContainer* pt;
    for(it = codeTabs.begin(); it != codeTabs.end(); ++it)
    {
        pt = it.value();
        delete pt;
    }

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


/* Sets the name displayed for a file, depending if the content has changed. */
/* (Displays a '*' when it is.) */
void MainWindow::setFileNameDisplay(QString filename, bool changed, ShaderLab::Shader shadertype)
{
    QMap<ShaderLab::Shader, ShaderCodeContainer *>::iterator it;
    it = codeTabs.find(shadertype);

    QString display = (changed ? QString("*") : QString()) + filename;

    if(it != codeTabs.end())
    {
        int ind = tabArea->indexOf(it.value());

        if(ind != -1)
        {
            tabArea->setTabText(ind, display);
        }
    }
}

/* Given the content, displays it on the apropriate tab. */
bool MainWindow::setShaderCode(const QString& code, ShaderLab::Shader shadertype)
{
    QMap<ShaderLab::Shader, ShaderCodeContainer *>::iterator it;

    it = codeTabs.find(shadertype);

    if(it != codeTabs.end())
    {
        it.value()->setText(code);
        return true;
    }
    else return false;
}

/* Displays or removes the desired tab from the screen. */
bool MainWindow::setVisibleShader(bool v, ShaderLab::Shader shadertype)
{
    QMap<ShaderLab::Shader, ShaderCodeContainer *>::iterator it;
    it = codeTabs.find(shadertype);

    if(it == codeTabs.end())
        return false;

    int ind = tabArea->indexOf(it.value());

    if(v == true)
    {
        if(tabArea->count() == 0)
        {
           tabArea->setVisible(true);
           find->setOkVisible(true);
        }
        if(ind == -1)
        {
            tabArea->insertTab(tabArea->count(), it.value(), QString());
            tabArea->setCurrentIndex(tabArea->count() -1);
            tabArea->setTabIcon(tabArea->count() - 1, QIcon(":/ico/running"));
        }
    }
    else
    {
        if(ind != -1)
            tabArea->removeTab(ind);
        if(tabArea->count() == 0)
        {
            tabArea->setVisible(false);
            find->setVisibility(false);
            find->setOkVisible(false);
        }
    }

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

/* Associated with the '' signal. */
/* */
void MainWindow::saveFile(void)
{
    ShaderCodeContainer* shaderTab = ((ShaderCodeContainer*)tabArea->currentWidget());
    if(shaderTab != NULL)
    {
        ShaderLab::Shader shadertype = shaderTab->getShaderType();
        emit saveFile(shadertype);
    }
}

/* Associated with the '' signal. */
/* */
void MainWindow::saveFileAsDialog(void)
{
    ShaderCodeContainer* shaderTab = ((ShaderCodeContainer*)tabArea->currentWidget());
    if(shaderTab != NULL)
    {
        ShaderLab::Shader shadertype = shaderTab->getShaderType();

        emit saveFileAs(shadertype);
    }
}


/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* +++++++++++++++++++++++++ Other methods +++++++++++++++++++++++++ */

/* */
void MainWindow::addShader(ShaderLab::Shader shadertype)
{
    ShaderCodeContainer *codeContainer = new ShaderCodeContainer(shadertype);
    codeTabs.insert(shadertype, codeContainer);

    connect(codeContainer, SIGNAL(textChanged(ShaderLab::Shader)),
            this, SIGNAL(shaderCodeChanged(ShaderLab::Shader)));

    connect(codeContainer, SIGNAL(doubleClicked(ShaderLab::Shader)),
            this, SIGNAL(changeActivationStatusClicked(ShaderLab::Shader)));
}

/* */
void MainWindow::closeEvent(QCloseEvent *event)
{
    emit programClose(event);
}

/* */
QString MainWindow::shaderCode(ShaderLab::Shader shadertype)
{
    QMap<ShaderLab::Shader, ShaderCodeContainer *>::iterator it;
    it = codeTabs.find(shadertype);

    if(it != codeTabs.end())
        return it.value()->getText();

    return QString();
}

/* */
bool MainWindow::visibleShader(ShaderLab::Shader shader)
{
    QMap<ShaderLab::Shader, ShaderCodeContainer *>::iterator it;
    it = codeTabs.find(shader);

    if(it != codeTabs.end())
    {
        int ind = tabArea->indexOf(it.value());
        if(ind == -1)
            return false;
        else
            return true;
    }
    else
        return false;
}

void MainWindow::setGLDisplay(GLDisplay * display)
{
    ui->verticalLayout->addWidget(display);
}

void MainWindow::setEnableShaderCode(ShaderLab::Shader shadertype, bool active)
{
    QMap<ShaderLab::Shader, ShaderCodeContainer *>::iterator it;
    it = codeTabs.find(shadertype);
    if(it == codeTabs.end())
        return;

    ShaderCodeContainer * sc = it.value();

    int ind = tabArea->indexOf(sc);

    if(ind != -1)
    {
        sc->setActivatedCode(active);
        if(active)
            tabArea->setTabIcon(ind, QIcon(":/ico/running"));
        else
            tabArea->setTabIcon(ind, QIcon(":/ico/stopped"));
    }
}

 QMenu* MainWindow::modelsMenu(void)
 {
    return ui->menu_Models;
 }

 void MainWindow::menuViewInsertAction( QAction* act)
 {
     ui->menuView->insertAction(0, act);
 }
