#include "SLObject.h"
#include "RenderController.h"
#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

#include "ChooseShaderDialog.h"
#include "InterfaceRequests.h"
#include "MainController.h"
#include "MainWindow.h"
#include "SLFile.h"
#include "Sphere.h"
#include "Project.h"
#include "EditorController.h"
#include "SLTabWidget.h"
#include "SLCodeContainer.h"

#define DEFFRAG "void main (){gl_FragColor = gl_Color;}"


/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* +++++++++++++++++ Constructors and destructors ++++++++++++++++++ */

MainController::MainController(MainWindow *mw, QObject *parent)
    : QObject(parent)
{
    mainWindow = mw;

    chooseShaderDialog = new ChooseShaderDialog(mainWindow);
    chooseShaderDialog->setModal(true);

    tabWidget = mw->createTabWidget();

    renderController = new RenderController(mainWindow);

    renderController->addSLObject(new SLObject(mainWindow));

    glSetup();

    connect(mainWindow, SIGNAL(programClose(QCloseEvent*)),
            this, SLOT(programCloseRequest(QCloseEvent*)));

    connect(mainWindow, SIGNAL(newShaderActionClicked()),
            this, SLOT(newShaderActionClicked()));

    connect(mainWindow, SIGNAL(openShaderActionClicked()),
            this, SLOT(openShaderActionClicked()));

    connect(mainWindow, SIGNAL(loadProject()),
            this, SLOT(loadProject()));

    connect(mainWindow, SIGNAL(saveShader()),
            this, SLOT(saveShader()));

    connect(mainWindow, SIGNAL(saveShaderAs()),
            this, SLOT(saveShaderAs()));

    connect(mainWindow, SIGNAL(newObject()),
            this, SLOT(newObject()));

    mainWindow->showMaximized();
}

void MainController::glSetup(void)
{
    ShaderLab * sl = ShaderLab::instance();
    sl->extensionsAnalise();

    if(!sl->criticalExtensionsEnabled())
    {
            QMessageBox::critical(mainWindow, tr("OpenGL features missing"),
                tr("The OpenGL extensions required to run this application are missing.\n") +
                tr("The program will now exit."));
            exit(0);
    }

    if(!sl->vertexShaderEnabled())
    {
        QMessageBox::warning(mainWindow, tr("OpenGL Vertex extension missing"),
            tr("The OpenGL Fragment extension required to run this application is missing.\n") +
            tr("The program will run without this feature."));
    }else
    {
        chooseShaderDialog->addButton(ShaderLab::Vertex);
    }

    if(!sl->fragmentShaderEnabled())
    {
        QMessageBox::warning(mainWindow, tr("OpenGL Fragment extension missing"),
            tr("The OpenGL Fragment extension required to run this application is missing.\n") +
            tr("The program will run without this feature."));
    }else
    {
        chooseShaderDialog->addButton(ShaderLab::Fragment);
    }

    if(!sl->geometryShaderEnabled())
    {
        QMessageBox::warning(mainWindow, tr("OpenGL Geometry extension missing"),
            tr("The OpenGL Geometry extension required to run this application is missing.\n") +
            tr("The program will run without this feature."));
        mainWindow->setEnableMenuGeometryShader(false);
    }else
    {
        chooseShaderDialog->addButton(ShaderLab::Geometry);
        mainWindow->setEnableMenuGeometryShader(true);
    }
}

MainController::~MainController()
{
    delete chooseShaderDialog;
    delete renderController;
}


/* Associated with the 'programClose' signal. */
/* Before ending the application, checks and manages all unsaved files. */
void MainController::programCloseRequest(QCloseEvent* event)
{
    if(!renderController->closeAllFiles())
        event->ignore();
}

void MainController::configureShader(ShaderLab::Shader shaderType, const QString& filePath)
{
    EditorController * editor = renderController->setShader(shaderType, filePath);
    if(editor == NULL)
        return;

    connect(mainWindow, SIGNAL(saveAll()), editor, SLOT(slot_saveFile()));

    tabWidget->addTab(editor);
    editor->codeContainer()->updateTabBar();
}

void MainController::newShaderActionClicked()
{
    int ret = chooseShaderDialog->exec();
    if(ret != QDialog::Accepted)
        return;

    configureShader(chooseShaderDialog->lastChosenShader());
}

void MainController::openShaderActionClicked()
{
    int ret = chooseShaderDialog->exec();
    if(ret != QDialog::Accepted)
        return;

    ShaderLab::Shader shaderType = chooseShaderDialog->lastChosenShader();

    QString filepath = InterfaceRequests::openShader(shaderType);

    if(filepath.isEmpty())
        return;

    if(!SLFile::isValid(filepath))
    {
        InterfaceRequests::openFileProblem(filepath);
        return;
    }

    configureShader(shaderType, filepath);

}

void MainController::saveShaderAs()
{
    if(tabWidget->count() > 0)
    {
        SLCodeContainer *cc = (SLCodeContainer *)tabWidget->currentWidget();
        cc->saveShaderAs();
    }
}

void MainController::saveShader()
{
    if(tabWidget->count() > 0)
    {
        SLCodeContainer *cc = (SLCodeContainer *)tabWidget->currentWidget();
        cc->saveShader();
    }
}

void MainController::loadProject(void)
{
    openProject(InterfaceRequests::openProject());
}

void MainController::openProject(const QString& filename)
{
    if(filename.isEmpty())
        return;

    Project *project = new Project;
    if(!project->load(filename))
    {
        InterfaceRequests::notLoadProject();
        delete project;
        return;
    }

    renderController->setModelById(project->getModelId());

    FORENABLEDSHADERS(shadertype)
    {
        QString fn = project->getFileName(shadertype);
        if(!fn.isEmpty())
        {
            if(!SLFile::isValid(fn))
                InterfaceRequests::openFileProblem(fn);
            else
                configureShader(shadertype,fn);
        }
    }

    renderController->setTexturesFromProject(project->getTextures());

    mainWindow->setSecondTitle(project->getAbsoluteFilePath());

    renderController->setProject(project);
}

void MainController::newObject()
{
    renderController->closeObject();
    renderController->addSLObject(new SLObject(mainWindow));
    mainWindow->setSecondTitle();
}

