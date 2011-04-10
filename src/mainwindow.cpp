#include <QDebug>
#include <QFile>
#include <QVBoxLayout>

#include "mainwindow.h"
#include "ui_mainwindow.h"

MainWindow::MainWindow(QWidget *parent) : QMainWindow(parent),
    ui(new Ui::MainWindow),  choiceDialog(new CodeChoiceDialog(this))
{
    ui->setupUi(this);
    tabArea = new QTabWidget(ui->centralWidget);
    tabArea->setTabsClosable(true);
    ui->horizontalLayout->addWidget(tabArea);

    ui->dockOutPutWidget->setVisible(false);
    ui->actionAplication_optput->setChecked(false);

    display = new GLDisplay();
    ui->verticalLayout->addWidget(display);

    connect(ui->actionOpen_Shader_Code, SIGNAL(triggered()), this, SLOT(openDialog()));
    connect(choiceDialog, SIGNAL(shader(ShaderLab::Shader)), this, SLOT(selectedShader(ShaderLab::Shader)));
    connect(tabArea, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTabRequest(int)));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(exitApplication()));
    connect(ui->actionRunShaders, SIGNAL(triggered()), this, SLOT(runShadersSelected()));
    connect(ui->menuView, SIGNAL(triggered(QAction*)), this, SLOT(viewMenuClicked(QAction*)));
    connect(ui->dockOutPutWidget, SIGNAL(visibilityChanged(bool)), this, SLOT(dockOutputVisibilityChange(bool)));
    connect(ui->dockRenderWidget, SIGNAL(visibilityChanged(bool)), this, SLOT(dockRenderVisibilityChange(bool)));
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
    delete choiceDialog;
    delete display;
    delete tabArea;
}

void MainWindow::openDialog(void)
{
    choiceDialog->open();
}

void MainWindow::selectedShader(ShaderLab::Shader sh)
{
    QString filename;

    switch(sh)
    {
        case ShaderLab::Vertex:
            filename = QFileDialog::getOpenFileName(this,tr("Open Vertex Shader Code"), "..", "*.vert");
            break;
        case ShaderLab::Fragment:
            filename = QFileDialog::getOpenFileName(this, tr("Open Fragment Shader Code"), "..", "*.frag");
            break;
    }

    if(!filename.isEmpty())
        emit selectedFile(filename, sh);
}

void MainWindow::closeTabRequest(int index)
{
    ShaderCodeContainer *pt = (ShaderCodeContainer*)tabArea->widget(index);

    emit closeTabRequest(pt->getShaderType());
}

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

QString MainWindow::shaderCode(ShaderLab::Shader shadertype)
{
    QMap<ShaderLab::Shader, ShaderCodeContainer *>::iterator it;
    it = codeTabs.find(shadertype);

    if(it != codeTabs.end())
        return it.value()->getText();

    return QString();
}

void MainWindow::setVisibleShader(bool v, ShaderLab::Shader shadertype)
{
    QMap<ShaderLab::Shader, ShaderCodeContainer *>::iterator it;
    it = codeTabs.find(shadertype);

    int ind = tabArea->indexOf(it.value());
    if(v == true)
    {
        if(ind == -1)
        {
            tabArea->insertTab(tabArea->count(), it.value(), tr(shaderToStr(it.key()).toAscii()));
            tabArea->setCurrentIndex(tabArea->count() -1);
        }

    }else
    {

        if(ind != -1)
        {
            it.value()->setText(QString());
            tabArea->removeTab(ind);
        }
    }
}

void MainWindow::setShaderCode(const QString& code, ShaderLab::Shader shadertype)
{
    QMap<ShaderLab::Shader, ShaderCodeContainer *>::iterator it;

    it = codeTabs.find(shadertype);

    if(it != codeTabs.end())
        it.value()->setText(code);
}

void MainWindow::exitApplication(void)
{
    close();
}

void MainWindow::runShadersSelected(void)
{
    emit runShaders();
}

void MainWindow::updateDisplay()
{
    display->updateGL();
}

void MainWindow::viewMenuClicked(QAction* act)
{
    if(act == ui->actionAplication_optput)
        ui->dockOutPutWidget->setVisible(act->isChecked());
    else if(act == ui->actionRender_area)
        ui->dockRenderWidget->setVisible(act->isChecked());

}

void MainWindow::setOutputText(const QString& s)
{
    ui->outputTextBox->setPlainText(s);
}

void MainWindow::dockOutputVisibilityChange(bool v)
{
    ui->actionAplication_optput->setChecked(v);
}

void MainWindow::dockRenderVisibilityChange(bool v)
{
    ui->actionRender_area->setChecked(v);
}

void MainWindow::addShader(ShaderLab::Shader shadertype)
{
    ShaderCodeContainer *codeContainer = new ShaderCodeContainer(shadertype);
    codeTabs.insert(shadertype, codeContainer);

    choiceDialog->addButton(shadertype);
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    emit programClose();
    event->accept();
}
