#include <QDebug>
#include <QMessageBox>

#include "maincontroller.h"
#include "mainwindow.h"
#include "filecontroller.h"
#include "global.h"
#include "sphere.h"


/* +++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++++ */
/* +++++++++++++++++ Constructors and destructors ++++++++++++++++++ */

MainController::MainController(MainWindow *mw, QObject *parent)
    : QObject(parent)
{
    mainWindow = mw;

    connect(mainWindow, SIGNAL(selectedFile(QString,ShaderLab::Shader)),
            this, SLOT(openShaderCode(QString,ShaderLab::Shader)));

    connect(mainWindow, SIGNAL(closeTabRequest(ShaderLab::Shader)),
            this, SLOT(closeShaderCode(ShaderLab::Shader)));

    connect(mainWindow, SIGNAL(runShaders()),
            this, SLOT(runAllActiveShaders()));

    connect(mainWindow, SIGNAL(programClose()),
            this, SLOT(programCloseRequest()));

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

    mainWindow->addShader(ShaderLab::Vertex);
    mainWindow->addShader(ShaderLab::Geometry);
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
void MainController::closeShaderCode(ShaderLab::Shader shaderType)
{
    QMap<ShaderLab::Shader, FileController*>::iterator it;
    it = fileControllers.find(shaderType);

    if(it != fileControllers.end())
    {
        FileController* fc = it.value();

        if(fc->getChanged())
        {
            if( mainWindow->saveRequest( fc->getFileName() ) )
            {
                if( fc->IsNew() )
                {
                    QString filename = mainWindow->saveAsRequest( it.key() );
                    if(filename.isEmpty())
                        return;

                    fc->saveAsNewFile( filename, mainWindow->shaderCode( it.key() ) );
                }
                else
                {
                    fc->save(mainWindow->shaderCode(it.key()));
                }
            }
        }

        fileControllers.erase(it);

        delete fc;

        mainWindow->setVisibleShader(false, shaderType);
    }
    else
    {
        qDebug() << "Deu pau!";
    }
}

/* Associated with the 'shaderCodeChanged' signal. */
/* Updates both the isChanged property and the displayed name of the file. */
void MainController::fileChanged(ShaderLab::Shader shadertype)
{
    QMap<ShaderLab::Shader, FileController*>::iterator it;
    it = fileControllers.find(shadertype);
    if(it == fileControllers.end())
        return;
    FileController *fc = it.value();

    fc->setChanged(true);

    mainWindow->setFileNameDisplay(fc->getFileName(), fc->getChanged(), shadertype);
}

/* Associated with the 'newShaderFile' signal. */
/* Creates a new FileController for a yet-non-existing file and displays the new empty file. */
/* Won't allow 2 opened files for the same shader type. */
void MainController::newShaderCode(ShaderLab::Shader shadertype)
{
    QMap<ShaderLab::Shader, FileController*>::iterator it;
    FileController *fc;

    it = fileControllers.find(shadertype);

    if(it == fileControllers.end())
    {
        fc = new FileController(shadertype);
        fileControllers.insert(shadertype, fc);

        mainWindow->setVisibleShader(true, shadertype);
        mainWindow->setShaderCode(QString(),  shadertype);
        mainWindow->setFileNameDisplay(fc->getFileName(), fc->getChanged(), shadertype);
    }
    else
    {
        QMessageBox::warning(mainWindow, QObject::tr("Failed to create file"),
                             QObject::tr(QString(shaderToStr(shadertype) + " code already opened.").toAscii()));
    }
}

/* Associated with the 'selectedFile' signal. */
/* Creates a new FileController for an existing file and displays its content for edition. */
/* Won't allow 2 opened files for the same shader type. */
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

        mainWindow->setVisibleShader(true, shaderType);
        mainWindow->setShaderCode(fileContent, shaderType);
        fc->setChanged(false);

        mainWindow->setFileNameDisplay(fc->getFileName(), fc->getChanged(), shaderType);

    }
    else
    {
        QMessageBox::warning(mainWindow, QObject::tr("Failed to open file"),
                             QObject::tr(QString(shaderToStrCap(shaderType) + " code already opened.").toAscii()));
    }
}

/* Associated with the 'programClose' signal. */
/* Before ending the application, checks and manages all unsaved files. */
void MainController::programCloseRequest(void)
{
    QMap<ShaderLab::Shader, FileController*>::iterator it;

    for(it = fileControllers.begin(); it != fileControllers.end(); ++it)
    {
        FileController* fc = it.value();

        if(fc->getChanged())
        {
            if( mainWindow->saveRequest( fc->getFileName(), fc->IsNew() ) )
            {
                if( fc->IsNew() )
                {
                    QString filename = mainWindow->saveAsRequest( it.key() );
                    if(filename.isEmpty())
                        return;

                    fc->saveAsNewFile( filename, mainWindow->shaderCode( it.key() ) );
                }
                else
                {
                    fc->save(mainWindow->shaderCode(it.key()));
                }
            }
            else continue;

        }

        fileControllers.erase(it);

        delete fc;

        mainWindow->setVisibleShader(false, it.key());
    }
}

/* Associated with the 'runShaders' signal. */
/* Compiles and runs all opened shader codes that are being edited, not the saved content. Also doesn't requires saving the code. */
/* Builds a compilation output that will be show to the programmer. */
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

        output += "==================== Compiling " + shaderToStr(shadertype) + " code ====================\n";

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
#ifdef QT47_CAPABLE
        program.setGeometryInputType(GL_TRIANGLE_STRIP);
        program.setGeometryOutputType(GL_TRIANGLE_STRIP);
#endif
        program.link();
        output += program.log();

        program.bind();
    }
    else
        output += "Due to problems, linking process could not be performed.";

    if(!thereIsCode)
        output = tr("No active shader code to compile.");

    mainWindow->setOutputText(output);
    emit updateGL();
}

/* Associated with the 'saveaAll' signal. */
/* Performs a saving routine for all unsaved files, distinguishin new ando old files. */
void MainController::saveAll()
{
    QMap<ShaderLab::Shader, FileController*>::iterator it;

    for(it = fileControllers.begin(); it != fileControllers.end(); ++it)
    {
        FileController* fc = it.value();

        if(fc->getChanged())
        {
            if( fc->IsNew() )
            {
                if( !mainWindow->saveRequest( fc->getFileName(), true ) )
                    continue;

                QString filename = mainWindow->saveAsRequest( it.key() );
                if(filename.isEmpty())
                    continue;

                fc->saveAsNewFile( filename, mainWindow->shaderCode( it.key() ) );
            }else
            {
                fc->save(mainWindow->shaderCode(it.key()));
            }

            mainWindow->setFileNameDisplay(fc->getFileName(), fc->getChanged(), it.key() );
        }
    }
}

/* Associated with the 'saveFile' signal. */
/* Called for a single instance (tab in teh UI. */
void MainController::saveFile(ShaderLab::Shader shadertype)
{
    QMap<ShaderLab::Shader, FileController*>::iterator it;
    bool saved = false;

    it = fileControllers.find(shadertype);

    if(it == fileControllers.end())
        return;

    FileController* fc = it.value();

    if( fc->IsNew() )
    {
        QString filename = mainWindow->saveAsRequest( it.key() );
        if(filename.isEmpty()) return;

        saved = fc->saveAsNewFile( filename, mainWindow->shaderCode( it.key() ) );
    }
    else
    {
        saved = fc->save(mainWindow->shaderCode(it.key()));
    }

    if(saved) mainWindow->setFileNameDisplay(fc->getFileName(), fc->getChanged(), shadertype);

}

/* Associated with the 'saveFileAs' signal. */
/* Only for existing files, creates a new file with the content of the screen and saves it. */
void MainController::saveFileAs(ShaderLab::Shader shadertype, const QString& filename, const QString& filecontent)
{
    QMap<ShaderLab::Shader, FileController*>::iterator it;
    it = fileControllers.find(shadertype);
    if(it == fileControllers.end())
        return;
    FileController *fc = it.value();

    if(fc->saveAsNewFile(filename, filecontent))
        mainWindow->setFileNameDisplay(fc->getFileName(), fc->getChanged(), shadertype);
}
