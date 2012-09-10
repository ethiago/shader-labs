#include "SLShaderProgramController.h"
#include "PrimitivesDialog.h"
#include "SLShaderProgram.h"
#include "MainWindow.h"

//constructors
SLShaderProgramController::SLShaderProgramController(MainWindow* mw, PrimitivesDialog* pd,  QObject *parent) :
    QObject(parent), primitivesDialog(pd), shaderProgram(0)
{
    primitivesDialog->setMaxOutputVertexOut(/*GL_MAX_GEOMETRY_OUTPUT_VERTICES*/128);

    connect(mw, SIGNAL(newPrimitiveDialog()),
            this, SLOT(showPrimitive()));

    connect(primitivesDialog, SIGNAL(valueChanged()),
            this, SLOT(valueChanged()));

    valueChanged();
}

//destructor
SLShaderProgramController::~SLShaderProgramController()
{
    disconnect(this);
}

//public methods
void SLShaderProgramController::setShaderProgram(SLShaderProgram * sp)
{
    shaderProgram = sp;
}

EditorController* SLShaderProgramController::setShader(ShaderLab::Shader type,const QString& filePath)
{
    //TODO: bring rule from SLShaderProgram to here
    if(shaderProgram != NULL)
        return shaderProgram->setShader(type, filePath);
    else
        return NULL;
}

bool SLShaderProgramController::closeAllShaders()
{
    return shaderProgram->closeAllShaders();
}

//public slots
void SLShaderProgramController::showPrimitive()
{
    primitivesDialog->show();
}

void SLShaderProgramController::valueChanged()
{
    if(shaderProgram != NULL)
    {
        shaderProgram->setGeometryOutputType(primitivesDialog->getCurrentOutputPrimitive());
        shaderProgram->setGeometryVertexCount(primitivesDialog->getOutputVertexCount());
    }
}

