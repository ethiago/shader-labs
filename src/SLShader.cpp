#include "SLShader.h"
#include "InterfaceRequests.h"
#include "EditorController.h"
#include "Object3D.h"

#define DEFFRAG "void main (){gl_FragColor = gl_Color;}"

SLShader::SLShader(MainWindow* mw, QObject *parent) :
    QObject(parent), vertexShader(NULL), geometryShader(NULL),
    fragmentShader(NULL), tessCtrlShader(NULL), tessEvalShader(NULL)
{
    primitivesDialog = new PrimitivesDialog(mw);

    connect(mw, SIGNAL(newPrimitiveDialog()), this, SLOT(showPrimitive()));
}

SLShader::~SLShader()
{
    if(vertexShader)
    {
        vertexShader->closeShaderCode();
        delete vertexShader;
    }
    if(geometryShader)
    {
        geometryShader->closeShaderCode();
        delete geometryShader;
    }
    if(fragmentShader)
    {
        fragmentShader->closeShaderCode();
        delete fragmentShader;
    }
    if(tessCtrlShader)
    {
        tessCtrlShader->closeShaderCode();
        delete tessCtrlShader;
    }
    if(tessEvalShader)
    {
        tessEvalShader->closeShaderCode();
        delete tessEvalShader;
    }
    //disconnect(0, 0, primitivesDialog, 0);
    delete primitivesDialog;
}

void SLShader::logProcess(const QString& log, ShaderLab::Shader st)
{
    m_log += "==================== Compiling "+ ShaderLab::shaderToStr(st) +" code ====================\n";
    QString tlog = log;

    if(tlog == "")
        m_log += "Successfull.\n";
    else
        m_log += tlog + "\n";
}

void SLShader::compileAndLink(QGLShaderProgram* program, Object3D* obj)
{
    program->removeAllShaders();
    program->release();

    bool atLeastOne = false;
    bool thereIsCode = false;
    bool fragmentOk = false;

    m_log = "";

    if(vertexShader && vertexShader->isActive())
    {
        thereIsCode = true;

        if(program->addShaderFromSourceCode(
                    QGLShader::Vertex, vertexShader->getContent()))
            atLeastOne = true;

        logProcess(program->log(), ShaderLab::Vertex);
    }

    if(geometryShader && geometryShader->isActive())
    {
        thereIsCode = true;

        if(program->addShaderFromSourceCode(
                    QGLShader::Geometry, geometryShader->getContent()))
            atLeastOne = true;

        logProcess(program->log(), ShaderLab::Geometry);
    }

    if(tessCtrlShader && tessCtrlShader->isActive())
    {
        thereIsCode = true;

        if(program->addShaderFromSourceCode(
                    QGLShader::TessellationCtrl, tessCtrlShader->getContent()))
            atLeastOne = true;

        logProcess(program->log(), ShaderLab::TessellationCtrl);
    }

    if(tessEvalShader && tessEvalShader->isActive())
    {
        thereIsCode = true;

        if(program->addShaderFromSourceCode(
                    QGLShader::TessellationEval, tessEvalShader->getContent()))
            atLeastOne = true;

        logProcess(program->log(), ShaderLab::TessellationEval);
    }


    if(fragmentShader && fragmentShader->isActive())
    {
        thereIsCode = true;

        if(program->addShaderFromSourceCode(
                    QGLShader::Fragment, fragmentShader->getContent()))
        {
            atLeastOne = true;
            fragmentOk = true;
        }

        logProcess(program->log(), ShaderLab::Fragment);
    }

    if(atLeastOne && !fragmentOk)
    {
        program->addShaderFromSourceCode(QGLShader::Fragment, QString(DEFFRAG));
    }

    m_log += "====================== Linking process ======================\n";
    if(atLeastOne)
    {

        ShaderLab * sl = ShaderLab::instance();
        if(sl->geometryShaderEnabled())
        {
            program->setGeometryInputType(obj->inputType());
            program->setGeometryOutputType(primitivesDialog->getCurrentOutputPrimitive());
        }

        program->link();
        m_log += program->log();
    }
    else
        m_log += "Due to problems, linking process could not be performed.";

    if(!thereIsCode)
        m_log = tr("No active shader code to compile.");
}

const QString& SLShader::log()
{
    return m_log;
}

EditorController* SLShader::setShader(const QString& filePath, ShaderLab::Shader shadertype)
{

    if(shadertype == ShaderLab::Vertex)
    {
        if(!closeShader(&vertexShader))
            return NULL;

        vertexShader = new EditorController(ShaderLab::Vertex, filePath);
        connect(vertexShader, SIGNAL(useless(EditorController*)),
                this, SLOT(unlessEditor(EditorController*)));
        return vertexShader;
    }else if(shadertype == ShaderLab::Geometry)
    {
        if(!closeShader(&geometryShader))
            return NULL;

        geometryShader = new EditorController(ShaderLab::Geometry, filePath);
        connect(geometryShader, SIGNAL(useless(EditorController*)),
                this, SLOT(unlessEditor(EditorController*)));
        return geometryShader;
    }else if(shadertype == ShaderLab::Fragment)
    {
        if(!closeShader(&fragmentShader))
            return NULL;

        fragmentShader = new EditorController(ShaderLab::Fragment, filePath);
        connect(fragmentShader, SIGNAL(useless(EditorController*)),
                this, SLOT(unlessEditor(EditorController*)));
        return fragmentShader;
    }else if(shadertype == ShaderLab::TessellationCtrl)
    {
        if(!closeShader(&tessCtrlShader))
            return NULL;

        tessCtrlShader = new EditorController(ShaderLab::TessellationCtrl, filePath);
        connect(tessCtrlShader, SIGNAL(useless(EditorController*)),
                this, SLOT(unlessEditor(EditorController*)));
        return tessCtrlShader;
    }else if(shadertype == ShaderLab::TessellationEval)
    {
        if(!closeShader(&tessEvalShader))
            return NULL;

        tessEvalShader = new EditorController(ShaderLab::TessellationEval, filePath);
        connect(tessEvalShader, SIGNAL(useless(EditorController*)),
                this, SLOT(unlessEditor(EditorController*)));
        return tessEvalShader;
    }

    return NULL;
}

void SLShader::showPrimitive()
{
    primitivesDialog->show();
}

void SLShader::unlessEditor(EditorController* editor)
{
    if(editor == vertexShader)
    {
        delete vertexShader;
        vertexShader = NULL;
    }
    else if(editor == geometryShader)
    {
        delete geometryShader;
        geometryShader = NULL;
    }
    else if(editor == fragmentShader)
    {
        delete fragmentShader;
        fragmentShader = NULL;
    }
    else if(editor == tessCtrlShader)
    {
        delete tessCtrlShader;
        tessCtrlShader = NULL;
    }
    else if(editor == tessEvalShader)
    {
         delete tessEvalShader;
         tessEvalShader = NULL;
    }
}

bool SLShader::isAnyNew()
{
    if(vertexShader && (vertexShader->getFile().IsNew()))
        return true;
    if(geometryShader && (geometryShader->getFile().IsNew()))
        return true;
    if(fragmentShader && (fragmentShader->getFile().IsNew()))
        return true;
    if(tessCtrlShader && (tessCtrlShader->getFile().IsNew()))
        return true;
    if(tessEvalShader && (tessEvalShader->getFile().IsNew()))
        return true;

    return false;
}

bool SLShader::saveAllShaders()
{
    bool all = true;
    if(vertexShader && !(vertexShader->saveFile()))
        all = false;
    if(geometryShader && !(geometryShader->saveFile()))
        all = false;
    if(fragmentShader && !(fragmentShader->saveFile()))
        all = false;
    if(tessCtrlShader && !(tessCtrlShader->saveFile()))
        all = false;
    if(tessEvalShader && !(tessEvalShader->saveFile()))
        all = false;


    return all;
}

bool SLShader::closeAllFiles()
{
    if(!closeShader(&vertexShader))
        return false;
    if(!closeShader(&geometryShader))
        return false;
    if(!closeShader(&fragmentShader))
        return false;
    if(!closeShader(&tessCtrlShader))
        return false;
    if(!closeShader(&tessEvalShader))
        return false;

    return true;
}

bool SLShader::closeShader(EditorController** shader)
{
    if(*shader)
    {
        if(!(*shader)->closeShaderCode())
            return false;

        delete *shader;
        *shader = NULL;
    }
    return true;
}

QString SLShader::getAbsoluteFilePath(ShaderLab::Shader shaderType)
{
    if(shaderType == ShaderLab::Vertex && vertexShader && !(vertexShader->getFile().IsNew()))
    {
        return vertexShader->getFile().getFilePath();
    }

    if(shaderType == ShaderLab::Geometry && geometryShader && !(geometryShader->getFile().IsNew()))
    {
        return geometryShader->getFile().getFilePath();
    }

    if(shaderType == ShaderLab::Fragment && fragmentShader && !(fragmentShader->getFile().IsNew()))
    {
        return fragmentShader->getFile().getFilePath();
    }

    if(shaderType == ShaderLab::TessellationCtrl && tessCtrlShader && !(tessCtrlShader->getFile().IsNew()))
    {
        return tessCtrlShader->getFile().getFilePath();
    }
    if(shaderType == ShaderLab::TessellationEval && tessEvalShader && !(tessEvalShader->getFile().IsNew()))
    {
        return tessEvalShader->getFile().getFilePath();
    }

    return QString();
}
