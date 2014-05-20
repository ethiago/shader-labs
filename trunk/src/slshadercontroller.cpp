#include "slshadercontroller.h"
#include "slShaderProgram2.h"
#include "slShaderCodes.h"
#include "slCodeContainer2.h"
#include "Global.h"
#include "InterfaceRequests.h"
#include "MainWindow.h"
#include "ChooseShaderDialog.h"
#include "SLFile.h"
#include "SLTabWidget.h"
#include "PrimitivesDialog.h"
#include "Project.h"
#include <QDebug>
#include <QDateTime>
#include <QGLContext>
#include <QMenu>

SLShaderController::SLShaderController(MainWindow *mw, ChooseShaderDialog *chooseShaderDialog) :
    QObject(NULL)
{
    ShaderLab *sl = ShaderLab::instance();

    m_primitives = new PrimitivesDialog();
    m_menuGeometrySettings = new QMenu("Geometry Shader");
    m_menuGeometrySettings->insertAction(NULL,m_primitives->toogleViewAction());
    if(!sl->geometryShaderEnabled())
        m_menuGeometrySettings->setEnabled(false);

    mw->addSettingsMenu(m_menuGeometrySettings);

    e_tabWidget = mw->getTabWidget();
    e_shaderProgram = NULL;
    e_shaderCodes = NULL;
    e_chooseShaderDialog = chooseShaderDialog;

    m_inputPrimitive = GL_TRIANGLES;

    unsigned int size = ShaderLab::shaderTypeList().size();
    for(unsigned int i = 0; i < size; ++i)
    {
        m_codeContainer.append(new SLCodeContainer2(ShaderLab::indexToShader(i), e_tabWidget));
        m_codeContainer[i]->hideTab();
        m_changed.append(false);

        connect(m_codeContainer[i], SIGNAL(closeRequestSignal(ShaderLab::Shader)),
                this, SLOT(closeRequestSignal(ShaderLab::Shader)));

        connect(m_codeContainer[i], SIGNAL(save(ShaderLab::Shader)),
                this, SLOT(save(ShaderLab::Shader)));

        connect(m_codeContainer[i], SIGNAL(textChanged(ShaderLab::Shader)),
                this, SLOT(textChanged(ShaderLab::Shader)));

        connect(m_codeContainer[i], SIGNAL(activateStatusChanged(ShaderLab::Shader, bool)),
                this, SLOT(activateStatusChanged(ShaderLab::Shader, bool)));
    }

    connect(e_tabWidget, SIGNAL(tabCloseRequested(int)),
            this, SLOT(tabCloseRequested(int)) );

    connect(mw, SIGNAL(newShaderActionClicked()),
            this, SLOT(newShader()));

    connect(mw, SIGNAL(saveShader()),
            this, SLOT(saveShader()));

    connect(mw, SIGNAL(saveShaderAs()),
            this, SLOT(saveShaderAs()));

    connect(mw, SIGNAL(openShaderActionClicked()),
            this, SLOT(openShader()));

    connect(mw, SIGNAL(runShaders()),
            this, SLOT(run()));

    connect(this, SIGNAL(logMessage(QString)),
            mw, SLOT(shaderLog(QString)));

}

SLShaderController::~SLShaderController()
{
    delete m_menuGeometrySettings;
    delete m_primitives;
}

void SLShaderController::newShader(ShaderLab::Shader shaderType, const QString& fileName)
{

    unsigned int idx = ShaderLab::shaderToIndex(shaderType);

    if(fileName.isEmpty())
    {
       e_shaderCodes->newShader(shaderType);
       m_codeContainer[idx]->setText(QString());
       m_codeContainer[idx]->setTabTitle("*" + e_shaderCodes->fileName(shaderType) );
       m_codeContainer[idx]->showTab();
       m_changed[idx] = true;
    }else
    {
        if(e_shaderCodes->openShader(shaderType, fileName))
        {
            m_codeContainer[idx]->setText(e_shaderCodes->lastSource(shaderType));
            m_codeContainer[idx]->setTabTitle(e_shaderCodes->fileName(shaderType));
            m_codeContainer[idx]->showTab();
            m_changed[idx] = false;
        }
    }
}

void SLShaderController::closeRequestSignal(ShaderLab::Shader shaderType)
{
    unsigned int idx = ShaderLab::shaderToIndex(shaderType);

    if( (e_shaderCodes->isNew(shaderType) || m_changed[idx]) )
    {
        InterfaceRequests::OperationState st =
                InterfaceRequests::saveRequestDialog( e_shaderCodes->fileName(shaderType), e_shaderCodes->isNew(shaderType));

        if(st == InterfaceRequests::Yes)
        {
            if(!save(shaderType))
                return;
        }else if(st == InterfaceRequests::Cancel)
            return;
    }

    m_codeContainer[idx]->setText("");
    m_codeContainer[idx]->setTabTitle("");
    m_codeContainer[idx]->hideTab();
    e_shaderCodes->closeShader(shaderType);
}

bool SLShaderController::save(ShaderLab::Shader shaderType)
{
    unsigned int idx = ShaderLab::shaderToIndex(shaderType);

    if(e_shaderCodes->isNew(shaderType))
        return saveAs(shaderType);

    e_shaderCodes->updateCode(shaderType, m_codeContainer[idx]->getText());
    if(e_shaderCodes->save(shaderType))
    {
        m_changed[idx] = false;
        m_codeContainer[idx]->setTabTitle(e_shaderCodes->fileName(shaderType));
        return true;
    }
    else
        return false;
}

bool SLShaderController::saveAs(ShaderLab::Shader shaderType)
{
    unsigned int idx = ShaderLab::shaderToIndex(shaderType);

    QString fn = InterfaceRequests::saveAsRequestDialog(shaderType);
    if(fn.isEmpty())
        return false;

    e_shaderCodes->updateCode(shaderType, m_codeContainer[idx]->getText());
    if(e_shaderCodes->saveAs(shaderType, fn) )
    {
        m_changed[idx] = false;
        m_codeContainer[idx]->setTabTitle(e_shaderCodes->fileName(shaderType));
        return true;
    }else
        return false;
}

void SLShaderController::textChanged(ShaderLab::Shader shaderType)
{
    if(e_shaderCodes == NULL)
        return;

    unsigned int idx = ShaderLab::shaderToIndex(shaderType);

    m_changed[idx] = true;
    e_shaderCodes->updateCode(shaderType, m_codeContainer[idx]->getText());

    if(!e_shaderCodes->isNew(shaderType))
    {
        m_codeContainer[idx]->setTabTitle("*" + e_shaderCodes->fileName(shaderType));
    }
}

void SLShaderController::activateStatusChanged(ShaderLab::Shader shaderType, bool v)
{
    e_shaderCodes->setActive(shaderType,v);
}

void SLShaderController::updateAll()
{
    if(e_shaderCodes == NULL)
        return;

    unsigned int size = ShaderLab::shaderTypeList().size();
    for(unsigned int i = 0; i < size; ++i)
    {
        if(!m_codeContainer[i]->isHidden())
        {
            ShaderLab::Shader shaderType = ShaderLab::indexToShader(i);
            e_shaderCodes->updateCode(shaderType, m_codeContainer[i]->getText());
        }
    }
}

void SLShaderController::hiddenAll()
{
    unsigned int size = ShaderLab::shaderTypeList().size();
    for(unsigned int i = 0; i < size; ++i)
    {
        m_codeContainer[i]->hideTab();
        m_changed[i] = false;
    }
}

void SLShaderController::setInputPrimitive(GLint inputPrimitive)
{
    m_inputPrimitive = inputPrimitive;
}

void SLShaderController::run()
{
    updateAll();

    QString log;

    log ="\n##############################################################\n";

    e_shaderCodes->compile();
    log += e_shaderCodes->log();

    if(e_shaderProgram->isLinked())
    {
        emit beforeUnLink(e_shaderProgram->programId());
    }
    if(e_shaderCodes->hasCompiledGeometry())
        e_shaderProgram->setGeometrySettings(m_inputPrimitive,
                                             m_primitives->getCurrentOutputPrimitive(),
                                             m_primitives->getOutputVertexCount());

    if(e_shaderProgram->link(e_shaderCodes->listForLink()))
    {
        emit afterLink(e_shaderProgram->programId());
    }

    log += e_shaderProgram->log();
    e_shaderCodes->clearShaders();

    log += "##############################################################\n";

    emit logMessage(log);

    ShaderLab *sl = ShaderLab::instance();
    sl->glContext()->updateGL();
}

bool SLShaderController::canOpenProject()const
{
    unsigned int size = ShaderLab::shaderTypeList().size();

    for(unsigned int i = 0; i < size; ++i)
    {
        if( !m_codeContainer[i]->isHidden())
        {
            return false;
        }
    }
    return true;
}

bool SLShaderController::canChangeObject()
{
    unsigned int size = ShaderLab::shaderTypeList().size();

    bool notSaved = false;
    for(unsigned int i = 0; i < size; ++i)
    {
        if( !m_codeContainer[i]->isHidden() && m_changed[i])
        {
            notSaved = true;
        }
    }

    if(notSaved)
    {
        InterfaceRequests::OperationState st =
            InterfaceRequests::notSavedCodeContinueAsk();

        if(st == InterfaceRequests::Cancel)
            return false;
    }

    updateAll();
    e_shaderProgram = NULL;
    e_shaderCodes = NULL;
    hiddenAll();
    return true;
}

void SLShaderController::setShaderObjects(SLShaderProgram2 * program, SLShaderCodes *shaders)
{
    updateAll();
    e_shaderProgram = program;
    e_shaderCodes = shaders;
    updateView();
}

void SLShaderController::updateView()
{
    hiddenAll();
    unsigned int size = ShaderLab::shaderTypeList().size();
    for(unsigned int i = 0; i < size; ++i)
    {
        ShaderLab::Shader type = ShaderLab::indexToShader(i);
        QString fn = e_shaderCodes->filePath(type);
        if(fn.isEmpty())
            continue;


        QString code = e_shaderCodes->lastSource(type);
        m_codeContainer[i]->setText(code);

        QString title = e_shaderCodes->fileName(type);
        m_changed[i] = e_shaderCodes->isChanged(type);
        if(m_changed[i])
            title = "*" + title;

        m_codeContainer[i]->setTabTitle(title);
        m_codeContainer[i]->setActive(e_shaderCodes->isActive(type));
        m_codeContainer[i]->showTab();
    }
}

void SLShaderController::newShader()
{
    int ret = e_chooseShaderDialog->exec();
    if(ret != QDialog::Accepted)
        return;

    ShaderLab::Shader shaderType = e_chooseShaderDialog->lastChosenShader();
    unsigned int idx = ShaderLab::shaderToIndex(shaderType);
    if(!m_codeContainer[idx]->isHidden())
    {
        InterfaceRequests::thereIsAlreadyAnOpenedShader(shaderType);
        return;
    }

    newShader(shaderType);
}

void SLShaderController::openShader()
{
    int ret = e_chooseShaderDialog->exec();
    if(ret != QDialog::Accepted)
        return;

    ShaderLab::Shader shaderType = e_chooseShaderDialog->lastChosenShader();
    unsigned int idx = ShaderLab::shaderToIndex(shaderType);
    if(!m_codeContainer[idx]->isHidden())
    {
        InterfaceRequests::thereIsAlreadyAnOpenedShader(shaderType);
        return;
    }

    QString filepath = InterfaceRequests::openShader(shaderType);

    if(filepath.isEmpty())
        return;

    if(!SLFile::isValid(filepath))
    {
        InterfaceRequests::openFileProblem(filepath);
        return;
    }

    newShader(shaderType, filepath);
}

void SLShaderController::saveShader()
{
    if(e_tabWidget->count() > 0)
    {
        SLCodeContainer2 *cc = (SLCodeContainer2 *)e_tabWidget->currentWidget();
        ShaderLab::Shader shaderType = cc->shaderType();
        save(shaderType);
    }
}

void SLShaderController::saveShaderAs()
{
    if(e_tabWidget->count() > 0)
    {
        SLCodeContainer2 *cc = (SLCodeContainer2 *)e_tabWidget->currentWidget();
        ShaderLab::Shader shaderType = cc->shaderType();
        saveAs(shaderType);
    }
}

void SLShaderController::tabCloseRequested(int tabIdx)
{
    SLCodeContainer2* cc = (SLCodeContainer2*)e_tabWidget->widget(tabIdx);
    closeRequestSignal(cc->shaderType());
}

void SLShaderController::projectOpened(Project* p)
{
    FORENABLEDSHADERS(shaderType)
    {
        QString fn = p->getFileName(shaderType);

        if(fn.isEmpty())
            continue;

        newShader(shaderType, fn);
    }
}
