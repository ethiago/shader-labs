#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

#include "TextureController.h" // Deve ser a primeira declaracao das classes proprias
#include "RenderController.h"
#include "ChooseShaderDialog.h"
#include "InterfaceRequests.h"
#include "MainController.h"
#include "MainWindow.h"
#include "FileController.h"
#include "Sphere.h"
#include "Project.h"


/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* +++++++++++++++++ Constructors and destructors ++++++++++++++++++ */

MainController::MainController(MainWindow *mw, QObject *parent)
    : QObject(parent)
{
    project = NULL;

    mainWindow = mw;

    chooseShaderDialog = new ChooseShaderDialog(mainWindow);
    chooseShaderDialog->setModal(true);

    renderController = new RenderController(mainWindow);
    textureController = new TextureController(mainWindow, renderController->getGLContext());

    connect(mainWindow, SIGNAL(closeTabRequest(ShaderLab::Shader)),
            this, SLOT(slot_closeShaderCode(ShaderLab::Shader)));

    connect(mainWindow, SIGNAL(runShaders()),
            this, SLOT(runAllActiveShaders()));

    connect(mainWindow, SIGNAL(programClose(QCloseEvent*)),
            this, SLOT(programCloseRequest(QCloseEvent*)));

    connect(mainWindow, SIGNAL(saveFile(ShaderLab::Shader)),
            this, SLOT(saveFile(ShaderLab::Shader)));

    connect(mainWindow, SIGNAL(shaderCodeChanged(ShaderLab::Shader)),
            this, SLOT(fileChanged(ShaderLab::Shader)));

    connect(mainWindow, SIGNAL(saveFileAs(ShaderLab::Shader)),
            this, SLOT(saveFileAs(ShaderLab::Shader)));

    connect(mainWindow, SIGNAL(saveAll()),
            this, SLOT(saveAll()));

    connect(mainWindow, SIGNAL(changeActivationStatusClicked(ShaderLab::Shader)),
            this, SLOT(changeTabActivationStatus(ShaderLab::Shader)));

    connect(mainWindow, SIGNAL(newShaderActionClicked()),
            this, SLOT(newShaderActionClicked()));

    connect(mainWindow, SIGNAL(openShaderActionClicked()),
            this, SLOT(openShaderActionClicked()));

    connect(mainWindow, SIGNAL(loadProject()),
            this, SLOT(loadProject()));

    connect(mainWindow, SIGNAL(saveAsProject()),
            this, SLOT(saveAsProject()));

    glSetup();

    mainWindow->showMaximized();
}

void MainController::glSetup(void)
{
    ShaderLab * sl = ShaderLab::instance();

    if((sl->extensions() & ShaderLab::FramebufferObject)  == 0 ||
            (sl->extensions() & ShaderLab::ShaderObjects) == 0)
    {
            QMessageBox::critical(mainWindow, tr("OpenGL features missing"),
                tr("The OpenGL extensions required to run this application are missing.\n") +
                tr("The program will now exit."));
            exit(0);
    }

    if((sl->extensions() & ShaderLab::VertexShader)  == 0)
    {
        QMessageBox::warning(mainWindow, tr("OpenGL Vertex extension missing"),
            tr("The OpenGL Fragment extension required to run this application are missing.\n") +
            tr("The program will run without this feature."));
    }else
    {
        mainWindow->addShader(ShaderLab::Vertex);
        chooseShaderDialog->addButton(ShaderLab::Vertex);
    }

    if( (sl->extensions() & ShaderLab::FragmentShader) == 0 )
    {
        QMessageBox::warning(mainWindow, tr("OpenGL Fragment extension missing"),
            tr("The OpenGL Fragment extension required to run this application are missing.\n") +
            tr("The program will run without this feature."));
    }else
    {
        mainWindow->addShader(ShaderLab::Fragment);
        chooseShaderDialog->addButton(ShaderLab::Fragment);
    }

    if( (sl->extensions() & ShaderLab::GeometryShader) == 0 )
    {
        QMessageBox::warning(mainWindow, tr("OpenGL Geometry extension missing"),
            tr("The OpenGL Geometry extension required to run this application are missing.\n") +
            tr("The program will run without this feature."));
    }else
    {
        mainWindow->addShader(ShaderLab::Geometry);
        chooseShaderDialog->addButton(ShaderLab::Geometry);
    }
}

MainController::~MainController()
{
    delete chooseShaderDialog;
    delete textureController;
    delete renderController;
    if(project != NULL)
        delete project;
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

    if(it == fileControllers.end())
        return true;

    FileController* fc = it.value();

    if( (fc->IsNew() || fc->getChanged()) )
    {
        InterfaceRequests::OperationState st = InterfaceRequests::saveRequestDialog(fc->getFileName(), fc->IsNew());
        if(st == InterfaceRequests::Yes)
            saveFile(shaderType);
        else if(st == InterfaceRequests::Cancel)
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

    if(fc != NULL)
    {
        fc->setChanged(true);

        mainWindow->setFileNameDisplay(fc->getFileName(), fc->getChanged(), shaderType);
    }
}


/* Associated with the 'programClose' signal. */
/* Before ending the application, checks and manages all unsaved files. */
void MainController::programCloseRequest(QCloseEvent* event)
{
    FORSHADERS(shadertype)
    {
        if(!closeShaderCode(shadertype))
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

        output += "==================== Compiling " + ShaderLab::shaderToStr(shaderType) + " code ====================\n";

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
        program.setGeometryInputType(renderController->getCurrentInputPrimitive());
        program.setGeometryOutputType(renderController->getCurrentOutputPrimitive());
#endif
        program.link();
        output += program.log();

        program.bind();

        textureController->applyTextures(&program);
    }
    else
        output += "Due to problems, linking process could not be performed.";

    if(!thereIsCode)
        output = tr("No active shader code to compile.");

    mainWindow->setOutputText(output);
    renderController->updateGL();
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
    saveFileBool(shaderType);
}

/* Associated with the 'saveFileAs' signal. */
/* Only for existing files, creates a new file with the content of the screen and saves it. */
void  MainController::saveFileAs(ShaderLab::Shader shaderType)
{
    FileController *fc = getFileControllerByShaderType(shaderType);

    QString filepath = InterfaceRequests::saveAsRequestDialog( shaderType );

    if(filepath.isEmpty()) return;
    fc->setFilePath(filepath);

    if(fc->save(mainWindow->shaderCode(shaderType)))
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

void MainController::newShaderActionClicked()
{
    int ret = chooseShaderDialog->exec();
    if(ret != QDialog::Accepted)
        return;

    ShaderLab::Shader shaderType = chooseShaderDialog->lastChosenShader();

    if(!closeShaderCode(shaderType))
            return;

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

void MainController::openShaderActionClicked()
{
    int ret = chooseShaderDialog->exec();
    if(ret != QDialog::Accepted)
        return;

    ShaderLab::Shader shaderType = chooseShaderDialog->lastChosenShader();
    QString fileContent;

    if(!closeShaderCode(shaderType))
        return;

    FileController *fc = getFileControllerByShaderType(shaderType);

    if(!fc)
    {
        QString filepath = QFileDialog::getOpenFileName(mainWindow,
                                  "Open " + ShaderLab::shaderToStr(shaderType) + " shader",
                                  "../..",
                                  "*" + ShaderLab::shaderToExt(shaderType));

        if(filepath.isEmpty())
            return;

        if(!FileController::isValid(filepath))
        {
            QMessageBox::warning(mainWindow, tr("Could not find file"),
                                 tr("Não foi possível encontrar o arquivo:\n")+
                                 filepath + "\n" +
                                 tr("para o ") + tr(ShaderLab::shaderToStr(shaderType).toAscii()) +
                                 tr(" shader"));
            return;
        }

        fc = new FileController(filepath, shaderType);
        fileControllers.insert(shaderType, fc);

        fileContent = fc->getFileContent();

        mainWindow->setVisibleShader(true, shaderType);
        mainWindow->setShaderCode(fileContent, shaderType);
        fc->setChanged(false);

        mainWindow->setFileNameDisplay(fc->getFileName(), fc->getChanged(), shaderType);

    }
}

void MainController::loadProject(void)
{
    QString filename = QFileDialog::getOpenFileName(mainWindow,tr("Open Project"), "..", "*.slp");
    if(filename.isEmpty())
        return;

    if(project != NULL)
        delete project;

    project = new Project;
    if(!project->load(filename))
    {
        qDebug() << "Deu Ruim!";
        return;
    }

    FORSHADERS(shadertype)
    {
        if(!closeShaderCode(shadertype))
        {
            delete project;
            project = NULL;
            return;
        }
    }

    FORSHADERS(shadertype)
    {
        openShader(shadertype, project->getFileName(shadertype));
    }
}

bool MainController::openShader(ShaderLab::Shader shaderType, QString filepath)
{
    QString fileContent;

    if(!closeShaderCode(shaderType))
        return false;

    FileController *fc = getFileControllerByShaderType(shaderType);

    if(!fc)
    {
        if(filepath.isEmpty())
            return false;

        if(!FileController::isValid(filepath))
        {
            QMessageBox::warning(mainWindow, tr("Could not find file"),
                                 tr("Não foi possível encontrar o arquivo:\n")+
                                 filepath + "\n" +
                                 tr("para o ") + tr(ShaderLab::shaderToStr(shaderType).toAscii()) +
                                 tr(" shader"));
            return false;
        }

        fc = new FileController(filepath, shaderType);
        fileControllers.insert(shaderType, fc);

        fileContent = fc->getFileContent();

        mainWindow->setVisibleShader(true, shaderType);
        mainWindow->setShaderCode(fileContent, shaderType);
        fc->setChanged(false);

        mainWindow->setFileNameDisplay(fc->getFileName(), fc->getChanged(), shaderType);

        return true;
    }

    return false;
}

bool MainController::saveFileBool(ShaderLab::Shader shaderType)
{
    FileController *fc = getFileControllerByShaderType(shaderType);

    if( fc->IsNew() )
    {
        QString filepath = InterfaceRequests::saveAsRequestDialog( shaderType );

        if(filepath.isEmpty()) return false;
        fc->setFilePath(filepath);
    }

    if( fc->getChanged() )
    {
        fc->save(mainWindow->shaderCode(shaderType));
        mainWindow->setFileNameDisplay(fc->getFileName(), fc->getChanged(), shaderType);
    }

    return true;
}

void MainController::saveAsProject(void)
{
    if(project != NULL)
        return; //lembrar de tratar

    for(FileIterator it = fileControllers.begin(); it != fileControllers.end(); ++it)
    {
        if(!saveFileBool(it.key()))
            return;
    }

    QString projectFileName = InterfaceRequests::saveProjectAsRequestDialog();

    if(projectFileName.isEmpty()) return;

    project = new Project();

    for(FileIterator it = fileControllers.begin(); it != fileControllers.end(); ++it)
    {
        project->includeShader(*(it.value()));
    }

    project->save(projectFileName);
}
