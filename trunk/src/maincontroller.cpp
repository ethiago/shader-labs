#include <QDebug>
#include <QMessageBox>

#include "maincontroller.h"
#include "mainwindow.h"
#include "filecontroller.h"
#include "global.h"

MainController::MainController(QObject *parent) : QObject(parent)
{
    mainWindow = new MainWindow();

    connect(mainWindow, SIGNAL(selectedFile(QString,ShaderLab::Shader)),
            this, SLOT(openShaderCode(QString,ShaderLab::Shader)));

    connect(mainWindow, SIGNAL(closeTabRequest(ShaderLab::Shader)),
            this, SLOT(closeShaderCode(ShaderLab::Shader)));

    connect(mainWindow, SIGNAL(runShaders()),
            this, SLOT(runAllActiveShaders()));

    connect(mainWindow, SIGNAL(programClose()),
            this, SLOT(programCloseRequest()));

    mainWindow->addShader(ShaderLab::Vertex);
    mainWindow->addShader(ShaderLab::Fragment);


    mainWindow->show();
}
MainController::~MainController()
{
    delete mainWindow;
}

void MainController::openShaderCode(const QString& filepath, ShaderLab::Shader shaderType)
{
    QString fileContent;
    QMap<ShaderLab::Shader, FileController*>::iterator it;
    FileController *fc;

    it = fileControllers.find(shaderType);

    if(it == fileControllers.end())
    {
        fc = new FileController(filepath, shaderType);
        fileControllers.insert(shaderType, fc);

        fileContent = fc->getFileContent();

        qDebug() << "setShader " << mainWindow->setShaderCode(fileContent, shaderType);
        qDebug() << "setVisible " << mainWindow->setVisibleShader(true, shaderType);
    }
    else
    {
        QMessageBox::warning(mainWindow, QObject::tr("Failed to open file"),
                             QObject::tr(QString(shaderToStr(shaderType) + " code already opened.").toAscii()));
    }
}

void MainController::closeShaderCode(ShaderLab::Shader shaderType)
{
    QMap<ShaderLab::Shader, FileController*>::iterator it;
    it = fileControllers.find(shaderType);

    if(it != fileControllers.end())
    {
        FileController* fc = it.value();
        fileControllers.erase(it);

        delete fc;

        mainWindow->setVisibleShader(false, shaderType);
    }
    else
    {
        qDebug() << "Deu pau!";
    }
}

void MainController::runAllActiveShaders(void)
{
    QMap<ShaderLab::Shader, FileController*>::iterator it;
    QGLShaderProgram program;
    QString output;

    bool atLeastOne = false;
    bool compOK, thereIsCode = false;
    ShaderLab::Shader shadertype;

    mainWindow->setOutputText(QString());

    for(it = fileControllers.begin(); it != fileControllers.end(); ++it)
    {
        thereIsCode = true;
        shadertype = it.key();

        output += "====================== Compiling " + shaderToStr(shadertype) + " code ======================\n";

        compOK = it.value()->compile(mainWindow->shaderCode(it.key()));
        if(compOK) program.addShader(it.value()->getShader());

        atLeastOne = atLeastOne || compOK;

        QString log = it.value()->log();
        if(log == "") output += "Successfull.\n";
        else output += log;

        output += "\n";
    }

    output += "====================== Linking process ======================\n";
    if(atLeastOne)
    {
        program.link();
        output += program.log();

        program.bind();
    }
    else
    {
        output += "Due to problems, linking process could not be performed.";
    }

    if(!thereIsCode)
        output = tr("No active shader code to compile.");

    mainWindow->setOutputText(output);
    mainWindow->updateDisplay();
}

void MainController::programCloseRequest(void)
{
    mainWindow->close();
    delete this;
}

































