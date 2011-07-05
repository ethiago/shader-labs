#include <QDebug>
#include <QMessageBox>

#include "maincontroller.h"
#include "mainwindow.h"
#include "FileController.h"
#include "TextureController.h"
#include "Global.h"
#include "sphere.h"


/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* +++++++++++++++++ Constructors and destructors ++++++++++++++++++ */

MainController::MainController(MainWindow *mw, QObject *parent)
    : QObject(parent)
{
    mainWindow = mw;
    textureController = NULL;

    connect(mainWindow, SIGNAL(selectedFile(QString,ShaderLab::Shader)),
            this, SLOT(openShaderCode(QString,ShaderLab::Shader)));

    connect(mainWindow, SIGNAL(closeTabRequest(ShaderLab::Shader)),
            this, SLOT(slot_closeShaderCode(ShaderLab::Shader)));

    connect(mainWindow, SIGNAL(runShaders()),
            this, SLOT(runAllActiveShaders()));

    connect(mainWindow, SIGNAL(programClose(QCloseEvent*)),
            this, SLOT(programCloseRequest(QCloseEvent*)));

    connect(mainWindow, SIGNAL(saveFile(ShaderLab::Shader)),
            this, SLOT(saveFile(ShaderLab::Shader)));

    connect(mainWindow, SIGNAL(newShaderFile(ShaderLab::Shader)),
            this, SLOT(newShaderCode(ShaderLab::Shader)));

    connect(mainWindow, SIGNAL(shaderCodeChanged(ShaderLab::Shader)),
            this, SLOT(fileChanged(ShaderLab::Shader)));

    connect(mainWindow, SIGNAL(saveFileAs(ShaderLab::Shader,QString,QString)),
            this, SLOT(saveFileAs(ShaderLab::Shader,QString,QString)));

    connect(mainWindow, SIGNAL(saveAll()),
            this, SLOT(saveAll()));

    connect(mainWindow, SIGNAL(changeActivationStatusClicked(ShaderLab::Shader)),
            this, SLOT(changeTabActivationStatus(ShaderLab::Shader)));

    mainWindow->addShader(ShaderLab::Vertex);
    /*mainWindow->addShader(ShaderLab::Geometry);*/
    mainWindow->addShader(ShaderLab::Fragment);

    mainWindow->showMaximized();
}

MainController::~MainController()
{
}


/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* +++++++++++++++++++++++++++++ Slots +++++++++++++++++++++++++++++ */

/* Associated with the 'closeTabRequest' signal. */
/* Reponsible for closing the tab of a shader program. Only when the content was changed, */
/* and giving different treatment for new and old files. */
void MainController::slot_closeShaderCode(ShaderLab::Shader shadertype)
{
    closeShaderCode(shadertype);
}

bool MainController::closeShaderCode(ShaderLab::Shader shaderType)
{
    QMap<ShaderLab::Shader, FileController*>::iterator it;
    it = fileControllers.find(shaderType);

    FileController* fc = it.value();

    if( (fc->IsNew() || fc->getChanged()) )
    {
        ShaderLab::OperationState st = mainWindow->saveRequest(fc->getFileName(), fc->IsNew());
        if(st == ShaderLab::Yes)
            saveFile(shaderType);
        else if(st == ShaderLab::Cancel)
            return false;
    }

    fileControllers.erase(it);
    program.removeShader(fc->getShader());

    delete fc;

    mainWindow->setVisibleShader(false, shaderType);

    return true;
}

/* Associated with the 'shaderCodeChanged' signal. */
/* Updates both the isChanged property and the displayed name of the file. */
void MainController::fileChanged(ShaderLab::Shader shaderType)
{
    FileController *fc = getFileControllerByShaderType(shaderType);

    fc->setChanged(true);

    mainWindow->setFileNameDisplay(fc->getFileName(), fc->getChanged(), shaderType);
}

/* Associated with the 'newShaderFile' signal. */
/* Creates a new FileController for a yet-non-existing file and displays the new empty file. */
/* Won't allow 2 opened files for the same shader type. */
void MainController::newShaderCode(ShaderLab::Shader shaderType)
{

    codeAlreadyOpenProcessor(shaderType);

    FileController *fc = getFileControllerByShaderType(shaderType);

    if(!fc)
    {
        fc = new FileController(shaderType);
        fileControllers.insert(shaderType, fc);

        mainWindow->setVisibleShader(true, shaderType);
        mainWindow->setShaderCode(QString(),  shaderType);
        mainWindow->setFileNameDisplay(fc->getFileName(), fc->getChanged(), shaderType);
    }
}

/* Associated with the 'selectedFile' signal. */
/* Creates a new FileController for an existing file and displays its content for edition. */
/* Won't allow 2 opened files for the same shader type. */
void MainController::openShaderCode(const QString& filepath, ShaderLab::Shader shaderType)
{
    QString fileContent;

    codeAlreadyOpenProcessor(shaderType);

    FileController *fc = getFileControllerByShaderType(shaderType);

    if(!fc)
    {
        fc = new FileController(filepath, shaderType);
        fileControllers.insert(shaderType, fc);

        fileContent = fc->getFileContent();

        mainWindow->setVisibleShader(true, shaderType);
        mainWindow->setShaderCode(fileContent, shaderType);
        fc->setChanged(false);

        mainWindow->setFileNameDisplay(fc->getFileName(), fc->getChanged(), shaderType);

    }
}

/* Associated with the 'programClose' signal. */
/* Before ending the application, checks and manages all unsaved files. */
void MainController::programCloseRequest(QCloseEvent* event)
{
    QMap<ShaderLab::Shader, FileController*>::iterator it;

    for(it = fileControllers.begin(); it != fileControllers.end(); ++it)
    {
        if(!closeShaderCode(it.key()))
        {
            event->ignore();
            return;
        }
    }

}

/* Associated with the 'runShaders' signal. */
/* Compiles and runs all opened shader codes that are being edited, not the saved content. Also doesn't requires saving the code. */
/* Builds a compilation output that will be show to the programmer. */
void MainController::runAllActiveShaders(void)
{
    QMap<ShaderLab::Shader, FileController*>::iterator it;
    QString output;

    program.removeAllShaders();
    program.release();

    bool atLeastOne = false;
    bool compOK, thereIsCode = false;
    ShaderLab::Shader shaderType;

    mainWindow->setOutputText(QString());

    for(it = fileControllers.begin(); it != fileControllers.end(); ++it)
    {
        FileController * fc = it.value();
        if(!fc->isActive())
            continue;

        thereIsCode = true;
        shaderType = it.key();

        output += "==================== Compiling " + shaderToStr(shaderType) + " code ====================\n";

        //QGLShader* shader = new QGLShader(ShaderLab::shaderToQGLShader(fc->getShaderType()));

        //compOK = shader->compileSourceCode(mainWindow->shaderCode(it.key()));
        compOK = fc->compile(mainWindow->shaderCode( shaderType ) );

        if(compOK)
        {
            //program.addShader(shader);

            program.addShader(fc->getShader());

            atLeastOne = true;
        }

        //QString log = shader->log();
        QString log = fc->log();

        if(log == "") output += "Successfull.\n";
        else output += log;

        output += "\n";

    }

    output += "====================== Linking process ======================\n";
    if(atLeastOne)
    {

#ifdef QT47_CAPABLE
        program.setGeometryInputType(GL_TRIANGLE_STRIP);
        program.setGeometryOutputType(GL_TRIANGLE_STRIP);
#endif
        program.link();
        output += program.log();

        program.bind();

        if(textureController != NULL)
        {
            textureController->applyTextures(&program);
        }
    }
    else
        output += "Due to problems, linking process could not be performed.";

    if(!thereIsCode)
        output = tr("No active shader code to compile.");

    mainWindow->setOutputText(output);
    emit updateGL();
}

/* Associated with the 'saveAll' signal. */
/* Performs a saving routine for all unsaved files, distinguishing new and old files. */
void MainController::saveAll()
{
    QMap<ShaderLab::Shader, FileController*>::iterator it;

    for(it = fileControllers.begin(); it != fileControllers.end(); ++it)
        saveFile(it.key());
}

/* Associated with the 'saveFile' signal. */
/* Called for a single instance (tab in the UI). */
/* Also used in the saveAll method. */
void MainController::saveFile(ShaderLab::Shader shaderType)
{
    FileController *fc = getFileControllerByShaderType(shaderType);

    if( fc->IsNew() )
    {
        QString filepath = mainWindow->saveAsRequest( shaderType );

        if(filepath.isEmpty()) return;
        fc->setFilePath(filepath);
    }

    if( fc->getChanged() )
    {
        fc->save(mainWindow->shaderCode(shaderType));
        mainWindow->setFileNameDisplay(fc->getFileName(), fc->getChanged(), shaderType);
    }
}

/* Associated with the 'saveFileAs' signal. */
/* Only for existing files, creates a new file with the content of the screen and saves it. */
void  MainController::saveFileAs(ShaderLab::Shader shaderType, const QString& filepath, const QString& filecontent)
{
    FileController *fc = getFileControllerByShaderType(shaderType);

    if(filepath.isEmpty()) return;
    fc->setFilePath(filepath);

    if(fc->save(filecontent))
        mainWindow->setFileNameDisplay(fc->getFileName(), fc->getChanged(), shaderType);
}

void MainController::changeTabActivationStatus(ShaderLab::Shader shaderType)
{
    FileController *fc = getFileControllerByShaderType(shaderType);

    fc->setActive(!fc->isActive());
    mainWindow->setEnableShaderCode(shaderType, fc->isActive());
}

FileController* MainController::getFileControllerByShaderType(ShaderLab::Shader shaderType)
{
    QMap<ShaderLab::Shader, FileController*>::iterator it;
    it = fileControllers.find(shaderType);

    if( it != fileControllers.end() )
        return it.value();
    else return NULL;
}

void MainController::codeAlreadyOpenProcessor(ShaderLab::Shader shadertype)
{
    FileController *fc = getFileControllerByShaderType(shadertype);

    if(fc != NULL)
    {
        closeShaderCode(shadertype);
    }
}

void MainController::setTextureController(TextureController *textureController)
{
    this->textureController = textureController;
}
