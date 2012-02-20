#ifndef SLSHADER_H
#define SLSHADER_H

#include <QObject>
#include <QGLShaderProgram>
#include <Global.h>
#include "MainWindow.h"
#include "PrimitivesDialog.h"
#include "SLFile.h"

class EditorController;

class SLShader : public QObject
{
    Q_OBJECT

    EditorController* vertexShader;
    EditorController* geometryShader;
    EditorController* fragmentShader;

    QGLShaderProgram m_program;

    QString m_log;

    PrimitivesDialog *primitivesDialog;

    void logProcess(const QString& log, ShaderLab::Shader st);
    bool closeShader(EditorController**);
public:
    explicit SLShader(MainWindow* mw, QObject *parent = 0);
    ~SLShader();

    void compileAndLink(QGLShaderProgram* m_program);
    const QString& log();
    EditorController* setShader(const QString& filePath, ShaderLab::Shader shadertype);
    bool isAnyNew();
    bool closeAllFiles();
    bool saveAllShaders();
    QString getAbsoluteFilePath(ShaderLab::Shader shaderType);

public slots:
    void showPrimitive();
    void unlessEditor(EditorController*);


};

#endif // SLSHADER_H
