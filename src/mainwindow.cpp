#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFile>
#include <QVBoxLayout>

MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),  choiceDialog(new CodeChoiceDialog(this))
{
    ui->setupUi(this);

    ui->dockOutPutWidget->setVisible(false);
    ui->actionAplication_optput->setChecked(false);

    openFile = new QFileDialog(this);
    openFile->setAcceptMode(QFileDialog::AcceptOpen);

    for(int i = 0; i < ui->tabWidget->count(); ++i)
        ui->tabWidget->removeTab(i);

    display = new GLDisplay();
    ui->verticalLayout->addWidget(display);

    connect(ui->actionOpen_Shader_Code, SIGNAL(triggered()), this, SLOT(openDialog()));
    connect(choiceDialog, SIGNAL(shader(ShaderLab::Shader)), this, SLOT(selectedShader(ShaderLab::Shader)));
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTabRequest(int)));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(exitApplication()));
    connect(ui->actionRunShaders, SIGNAL(triggered()), this, SLOT(runShadersSelected()));
    connect(ui->menuView, SIGNAL(triggered(QAction*)), this, SLOT(viewMenuClicked(QAction*)));
    connect(ui->dockOutPutWidget, SIGNAL(visibilityChanged(bool)), this, SLOT(dockOutputVisibilityChange(bool)));
    connect(ui->dockRenderWidget, SIGNAL(visibilityChanged(bool)), this, SLOT(dockRenderVisibilityChange(bool)));
}

MainWindow::~MainWindow()
{
    delete ui;
    delete openFile;
    delete choiceDialog;
}

void MainWindow::openDialog(void)
{
    choiceDialog->open();
}


void MainWindow::vertexFileSelected(const QString& vertexCodeFileName)
{
    emit selectedFile(vertexCodeFileName, ShaderLab::Vertex);
}

void MainWindow::fragmentFileSelected(const QString& fragmentCodeFileName)
{
    emit selectedFile(fragmentCodeFileName, ShaderLab::Fragment);
}


void MainWindow::selectedShader(ShaderLab::Shader sh)
{
    switch(sh)
    {
        case ShaderLab::Vertex:
            openFile->setWindowTitle("Open Vertex Shader Code");
            openFile->setFileMode(QFileDialog::ExistingFile);
            openFile->setDirectory("..");
            openFile->setFilter("*.vert");
            openFile->open(this, SLOT(vertexFileSelected(const QString&)));
            break;
        case ShaderLab::Fragment:
            openFile->setWindowTitle("Open Fragment Shader Code");
            openFile->setFileMode(QFileDialog::ExistingFile);
            openFile->setDirectory("..");
            openFile->setFilter("*.frag");
            openFile->open(this, SLOT(fragmentFileSelected(const QString&)));
            break;
    }
}

void MainWindow::closeTabRequest(int index)
{
    QWidget *pt = ui->tabWidget->widget(index);

    if(pt == ui->vertexTab)
        emit closeTabRequest(ShaderLab::Vertex);
    else if(pt == ui->fragmentTab)
        emit closeTabRequest(ShaderLab::Fragment);
}

bool MainWindow::visibleShader(ShaderLab::Shader shader)
{
    QWidget *pt;

    if(shader == ShaderLab::Vertex)
        pt = ui->vertexTab;
    else if(shader == ShaderLab::Fragment)
        pt = ui->fragmentTab;


    int ind = ui->tabWidget->indexOf(pt);

    if(ind == -1)
        return false;
    else
        return true;

}

QString MainWindow::shaderCode(ShaderLab::Shader shader)
{
    if(shader == ShaderLab::Vertex)
        return ui->vertexCodeBox->toPlainText();
    else if(shader == ShaderLab::Fragment)
        return ui->fragmentCodeBox->toPlainText();

}

void MainWindow::setVisibleShader(bool v, ShaderLab::Shader shader)
{
    QWidget *pt;
    QString label;

    if(shader == ShaderLab::Vertex)
    {
        pt = ui->vertexTab;
        label = tr("vertex");
    }else if(shader == ShaderLab::Fragment)
    {
        pt = ui->fragmentTab;
        label = tr("fragment");
    }

    int ind = ui->tabWidget->indexOf(pt);
    if(v == true)
    {
        if(ind == -1)
            ui->tabWidget->insertTab(ui->tabWidget->count(), pt, label);
    }else
    {

        if(ind != -1)
            ui->tabWidget->removeTab(ind);
    }
}

void MainWindow::setShaderCode(const QString& code, ShaderLab::Shader shader)
{
    if(shader == ShaderLab::Vertex)
        ui->vertexCodeBox->setText(code);
    if(shader == ShaderLab::Fragment)
        ui->fragmentCodeBox->setText(code);
}

void MainWindow::exitApplication(void)
{
    exit(0);
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
