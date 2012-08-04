#ifndef SLSHADERPROGRAMCONTROLLER_H
#define SLSHADERPROGRAMCONTROLLER_H

#include <QObject>
#include "Global.h"

class PrimitivesDialog;
class SLShaderProgram;
class EditorController;
class MainWindow;

class SLShaderProgramController : public QObject
{
    Q_OBJECT

    PrimitivesDialog * primitivesDialog;
    SLShaderProgram * shaderProgram;

public:
    explicit SLShaderProgramController(MainWindow* mw, PrimitivesDialog*, QObject *parent = 0);
    ~SLShaderProgramController();

    void setShaderProgram(SLShaderProgram * sp);
    EditorController* setShader(ShaderLab::Shader, const QString&);
    bool closeAllShaders();

signals:

public slots:
    void showPrimitive();
    void valueChanged();
};

#endif // SLSHADERPROGRAMCONTROLLER_H
