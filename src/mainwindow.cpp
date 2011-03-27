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

    connect(ui->actionOpen_Shader_Code, SIGNAL(triggered()), this, SLOT(openDialog()));
    connect(choiceDialog, SIGNAL(shader(ShaderLab::Shader)), this, SLOT(selectedShader(ShaderLab::Shader)));
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
    ui->vertexTab->setVisible(v);
}

void MainWindow::setVisibleFragmentTab(bool v)
{
    ui->fragmentTab->setVisible(v);
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
    return ui->vertexTab->isVisible();
}

bool MainWindow::visibleFragmentTab(void)
{
    return ui->fragmentTab->isVisible();
}

QString MainWindow::vertexCode(void)
{
    return ui->vertexCodeBox->toPlainText();
}

QString MainWindow::fragmentCode(void)
{
    return ui->fragmentCodeBox->toPlainText();
}
