#include "mainwindow.h"
#include "ui_mainwindow.h"
#include <QDebug>
#include <QFile>


MainWindow::MainWindow(QWidget *parent) :
    QMainWindow(parent),
    ui(new Ui::MainWindow),  choiceDialog(new CodeChoiceDialog(this))
{
    ui->setupUi(this);

    openFile = new QFileDialog(this);
    openFile->setAcceptMode(QFileDialog::AcceptOpen);

    for(int i = 0; i < ui->tabWidget->count(); ++i)
        ui->tabWidget->removeTab(i);

    display = new GLDisplay(ui->dockWidget);

    connect(ui->actionOpen_Shader_Code, SIGNAL(triggered()), this, SLOT(openDialog()));
    connect(choiceDialog, SIGNAL(shader(ShaderLab::Shader)), this, SLOT(selectedShader(ShaderLab::Shader)));
    connect(ui->tabWidget, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTabRequest(int)));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(exitApplication()));
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

void MainWindow::setVisibleVertexTab(bool v)
{
    int ind = ui->tabWidget->indexOf(ui->vertexTab);

    if(v == true)
    {
        if(ind == -1)
        {
            ui->tabWidget->insertTab(ui->tabWidget->count(), ui->vertexTab, tr("vertex"));
        }
    }else
    {

        if(ind != -1)
        {
            ui->tabWidget->removeTab(ind);
        }
    }
}

void MainWindow::setVisibleFragmentTab(bool v)
{
    int ind = ui->tabWidget->indexOf(ui->fragmentTab);

    if(v == true)
    {
        if(ind == -1)
        {
            ui->tabWidget->insertTab(ui->tabWidget->count(), ui->fragmentTab, tr("fragment"));
        }
    }else
    {

        if(ind != -1)
        {
            ui->tabWidget->removeTab(ind);
        }
    }
}

void MainWindow::setVertexCode(const QString& code)
{
    ui->vertexCodeBox->setText(code);
}

void MainWindow::setFragmentCode(const QString& code)
{
    ui->fragmentCodeBox->setText(code);
}

bool MainWindow::visibleVertexTab(void)
{
    int ind = ui->tabWidget->indexOf(ui->vertexTab);

    if(ind == -1)
        return false;
    else
        return true;
}

bool MainWindow::visibleFragmentTab(void)
{
    int ind = ui->tabWidget->indexOf(ui->fragmentTab);

    if(ind == -1)
        return false;
    else
        return true;
}

QString MainWindow::vertexCode(void)
{
    return ui->vertexCodeBox->toPlainText();
}

QString MainWindow::fragmentCode(void)
{
    return ui->fragmentCodeBox->toPlainText();
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
