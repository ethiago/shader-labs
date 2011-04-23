
#include <QFile>
#include <QVBoxLayout>
#include <QMessageBox>

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
    connect(ui->action_New_Shader_Code, SIGNAL(triggered()), this, SLOT(newDialog()));
    connect(tabArea, SIGNAL(tabCloseRequested(int)), this, SLOT(closeTabRequest(int)));
    connect(ui->actionExit, SIGNAL(triggered()), this, SLOT(exitApplication()));
    connect(ui->actionRunShaders, SIGNAL(triggered()), this, SLOT(runShadersSelected()));
    connect(ui->menuView, SIGNAL(triggered(QAction*)), this, SLOT(viewMenuClicked(QAction*)));
    connect(ui->dockOutPutWidget, SIGNAL(visibilityChanged(bool)), this, SLOT(dockOutputVisibilityChange(bool)));
    connect(ui->dockRenderWidget, SIGNAL(visibilityChanged(bool)), this, SLOT(dockRenderVisibilityChange(bool)));
    connect(ui->action_Save_Shader_Code, SIGNAL(triggered()), this, SLOT(saveFile()));
    connect(ui->actionSave_File_As, SIGNAL(triggered()), this, SLOT(saveFileAsDialog()));
    connect(ui->actionSave_All_Shaders, SIGNAL(triggered()), this, SIGNAL(saveAll()));
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
    connect(choiceDialog, SIGNAL(shader(ShaderLab::Shader)), this, SLOT(selectedShaderOpenDialog(ShaderLab::Shader)));
    choiceDialog->open();
}

void MainWindow::selectedShaderOpenDialog(ShaderLab::Shader sh)
{
    QString filename;

    disconnect(choiceDialog, SIGNAL(shader(ShaderLab::Shader)), this, SLOT(selectedShaderOpenDialog(ShaderLab::Shader)));

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


void MainWindow::newDialog(void)
{
    connect(choiceDialog, SIGNAL(shader(ShaderLab::Shader)), this, SLOT(selectedShaderNewDialog(ShaderLab::Shader)));
    choiceDialog->open();
}

void MainWindow::selectedShaderNewDialog(ShaderLab::Shader shadertype)
{
    disconnect(choiceDialog, SIGNAL(shader(ShaderLab::Shader)), this, SLOT(selectedShaderNewDialog(ShaderLab::Shader)));

    emit newShaderFile(shadertype);
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

bool MainWindow::setVisibleShader(bool v, ShaderLab::Shader shadertype)
{
    QMap<ShaderLab::Shader, ShaderCodeContainer *>::iterator it;
    it = codeTabs.find(shadertype);

    if(it == codeTabs.end())
        return false;

    int ind = tabArea->indexOf(it.value());
    if(v == true)
    {
        if(ind == -1)
        {
            tabArea->insertTab(tabArea->count(), it.value(), QString());
            tabArea->setCurrentIndex(tabArea->count() -1);
        }

    }
    else
    {
        if(ind != -1)
        {
            //it.value()->setText(QString());
            tabArea->removeTab(ind);
        }
    }

    return true;
}

bool MainWindow::setShaderCode(const QString& code, ShaderLab::Shader shadertype)
{
    QMap<ShaderLab::Shader, ShaderCodeContainer *>::iterator it;

    it = codeTabs.find(shadertype);

    if(it != codeTabs.end())
    {
        it.value()->setText(code);
        tabArea->setTabText(tabArea->currentIndex(), tr(shaderToStr(shadertype).toAscii()));
        return true;
    }else
        return false;
}

void MainWindow::exitApplication(void)
{
    close();
}

void MainWindow::runShadersSelected(void)
{
    emit runShaders();
}

bool MainWindow::updateDisplay()
{
    display->updateGL();

    return true;
}

void MainWindow::viewMenuClicked(QAction* act)
{
    if(act == ui->actionAplication_optput)
        ui->dockOutPutWidget->setVisible(act->isChecked());
    else if(act == ui->actionRender_area)
        ui->dockRenderWidget->setVisible(act->isChecked());

}

bool MainWindow::setOutputText(const QString& s)
{
    ui->outputTextBox->setPlainText(s);

    return true;
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

    connect(codeContainer, SIGNAL(textChanged(ShaderLab::Shader)), this, SLOT(textChanged(ShaderLab::Shader)));

    choiceDialog->addButton(shadertype);
}

QString MainWindow::getOutputText(void)
{
    return ui->outputTextBox->toPlainText();
}

void MainWindow::closeEvent(QCloseEvent *event)
{
    emit programClose();
    event->accept();
}

void MainWindow::saveFile(void)
{
    ShaderCodeContainer* shaderTab = ((ShaderCodeContainer*)tabArea->currentWidget());
    if(shaderTab != NULL)
    {
        ShaderLab::Shader shadertype = shaderTab->getShaderType();
        emit saveFile(shadertype);
    }
}

void MainWindow::textChanged(ShaderLab::Shader shadertype)
{
    emit shaderCodeChanged(shadertype);
}

void MainWindow::setFileNameDisplay(QString filename, bool changed, ShaderLab::Shader shadertype)
{
    QMap<ShaderLab::Shader, ShaderCodeContainer *>::iterator it;

    QString display = (changed ? QString("*") : QString()) + filename;

    it = codeTabs.find(shadertype);

    if(it != codeTabs.end())
    {
        int ind = tabArea->indexOf(it.value());

        if(ind != -1)
            tabArea->setTabText(ind, display);
    }
}

bool MainWindow::saveRequest(const QString& filename)
{
    QMessageBox::StandardButton ok = QMessageBox::Yes;
    QMessageBox::StandardButton no = QMessageBox::No;
    QMessageBox::StandardButton bt;

    bt = QMessageBox::question(this, "Save File", "The file " + filename + " has been modified.\n Do you ant to save?",
                               no | ok,
                               ok);


    if(bt == ok)
        return true;
    else
        return false;
}

QString MainWindow::saveAsRequest(ShaderLab::Shader shader)
{
    QString filename = QFileDialog::getSaveFileName(this, "Save As", "..");

    if(filename.isEmpty())
        return filename;

    switch(shader)
    {
        case ShaderLab::Vertex:
            filename += ".vert";
            break;
        case ShaderLab::Fragment:
            filename += ".frag";
            break;
    }

    return filename;
}

void MainWindow::saveFileAsDialog(void)
{
    ShaderCodeContainer* shaderTab = ((ShaderCodeContainer*)tabArea->currentWidget());
    if(shaderTab != NULL)
    {
        ShaderLab::Shader shadertype = shaderTab->getShaderType();

        emit saveFileAs(shadertype, saveAsRequest(shadertype), shaderCode(shadertype));
    }
}
