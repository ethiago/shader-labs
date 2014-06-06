#include "gl3w.h"
#include "MainController.h"
#include "rendercontroller2.h"
#include "ChooseShaderDialog.h"
#include "InterfaceRequests.h"
#include "MainWindow.h"
#include "slshadercontroller.h"
#include "slTextureController.h"
#include "slProjectController.h"
#include "objectcontroller.h"

#include <QDebug>
#include <QMessageBox>
#include <QFileDialog>

/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* +++++++++++++++++ Constructors and destructors ++++++++++++++++++ */

MainController::MainController(MainWindow *mw, QObject *parent)
    : QObject(parent)
{
    e_mainWindow = mw;

    m_chooseShaderDialog = new ChooseShaderDialog(e_mainWindow);
    m_chooseShaderDialog->setModal(true);

    m_shaderController = new SLShaderController(e_mainWindow, m_chooseShaderDialog);
    m_textureController = new SLTextureController(e_mainWindow);

    connect( m_shaderController, SIGNAL(afterLink(GLuint)),
             m_textureController, SLOT(afterLink(GLuint)) );

    m_renderController = new RenderController2(e_mainWindow, m_shaderController, m_textureController);
    m_projectController = new SLProjectController(m_renderController->getScene(),
                                                  m_shaderController,
                                                  m_textureController,
                                                  m_renderController);

    connect(e_mainWindow, SIGNAL(loadProject()),
            m_projectController, SLOT(open()) );
    connect(e_mainWindow, SIGNAL(saveProject()) ,
            m_projectController, SLOT(save()));
    connect(e_mainWindow, SIGNAL(saveAsProject()),
            m_projectController, SLOT(saveAs()));
    connect(e_mainWindow, SIGNAL(closeProject()),
            m_projectController, SLOT(close()) );
    connect(m_projectController, SIGNAL(projectNameChanged(QString)),
            e_mainWindow, SLOT(setSecondTitle(QString)) );
    connect(m_renderController, SIGNAL(objectChanged()),
            m_projectController, SLOT(currentChanged()));

    glSetup();

    connect(e_mainWindow, SIGNAL(programClose(QCloseEvent*)),
            this, SLOT(programCloseRequest(QCloseEvent*)));


//    connect(mainWindow, SIGNAL(loadProject()),
//            this, SLOT(loadProject()));


    connect(e_mainWindow, SIGNAL(newObject()),
            this, SLOT(newObject()));

    e_mainWindow->showMaximized();
}

MainController::~MainController()
{
    delete m_chooseShaderDialog;
    delete m_renderController;
    delete m_shaderController;
    delete m_textureController;
    delete m_projectController;
}

void MainController::glSetup(void)
{
    ShaderLab * sl = ShaderLab::instance();
    sl->extensionsAnalise();

    if(!sl->criticalExtensionsEnabled())
    {
            QMessageBox::critical(e_mainWindow, tr("OpenGL features missing"),
                tr("The OpenGL extensions required to run this application are missing.\n") +
                tr("The program will now exit."));
            exit(0);
    }

    if(!sl->vertexShaderEnabled())
    {
        QMessageBox::warning(e_mainWindow, tr("OpenGL Vertex extension missing"),
            tr("The OpenGL Fragment extension required to run this application is missing.\n") +
            tr("The program will run without this feature."));
    }else
    {
        m_chooseShaderDialog->addButton(ShaderLab::Vertex);
    }

    if(!sl->fragmentShaderEnabled())
    {
        QMessageBox::warning(e_mainWindow, tr("OpenGL Fragment extension missing"),
            tr("The OpenGL Fragment extension required to run this application is missing.\n") +
            tr("The program will run without this feature."));
    }else
    {
        m_chooseShaderDialog->addButton(ShaderLab::Fragment);
    }

    if(!sl->geometryShaderEnabled())
    {
        QMessageBox::warning(e_mainWindow, tr("OpenGL Geometry extension missing"),
            tr("The OpenGL Geometry extension required to run this application is missing.\n") +
            tr("The program will run without this feature."));
        //mainWindow->setEnableMenuGeometryShader(false);
    }else
    {
        m_chooseShaderDialog->addButton(ShaderLab::Geometry);
        //mainWindow->setEnableMenuGeometryShader(true);
    }

    if(!sl->tesselationShaderEnable())
    {
        QMessageBox::warning(e_mainWindow, tr("OpenGL Tessellation extension missing"),
            tr("The OpenGL Tessellation extension required to run this application is missing.\n") +
            tr("The program will run without this feature."));
    }else
    {
        m_chooseShaderDialog->addButton(ShaderLab::TessellationCtrl);
        m_chooseShaderDialog->addButton(ShaderLab::TessellationEval);
    }
}

/* Associated with the 'programClose' signal. */
/* Before ending the application, checks and manages all unsaved files. */
void MainController::programCloseRequest(QCloseEvent* event)
{
    if(m_projectController->isOpened())
    {
         InterfaceRequests::OperationState st;
         st = InterfaceRequests::openedProjectContinueAsk();
         if(st == InterfaceRequests::Cancel)
         {
             event->ignore();
             return;
         }
    }

    if(!m_shaderController->canChangeObject())
        event->ignore();
}

void MainController::newObject()
{
    m_renderController->newSLObject();
    e_mainWindow->setSecondTitle("");
}

