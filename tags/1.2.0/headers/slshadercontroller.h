#ifndef SLSHADERCONTROLLER_H
#define SLSHADERCONTROLLER_H

#include <QObject>

#include "Global.h"
#include "gl3w.h"


class SLShaderProgram2;
class SLCodeContainer2;
class SLShaderCodes;
class MainWindow;
class ChooseShaderDialog;
class SLTabWidget;
class PrimitivesDialog;
class QMenu;
class Project;

class SLShaderController : public QObject
{
    Q_OBJECT

    QList<SLCodeContainer2 *> m_codeContainer;
    QList<bool> m_changed;

    SLShaderProgram2 * e_shaderProgram;
    SLShaderCodes * e_shaderCodes;
    ChooseShaderDialog *e_chooseShaderDialog;
    SLTabWidget * e_tabWidget;
    PrimitivesDialog * m_primitives;

    QMenu* m_menuGeometrySettings;

    GLint m_inputPrimitive;

    void updateAll();
    void hiddenAll();
    void updateView();

public:
    explicit SLShaderController(MainWindow *mw, ChooseShaderDialog *chooseShaderDialog);
    ~SLShaderController();

    void newShader(ShaderLab::Shader shaderType, const QString& fileName = QString());
    bool canChangeObject();
    bool canOpenProject()const;
    void setShaderObjects(SLShaderProgram2 * program, SLShaderCodes* shaders);
    void setInputPrimitive(GLint inputPrimitive);

signals:
    void logMessage(QString log);
    void beforeUnLink(GLuint);
    void afterLink(GLuint);
    
public slots:
    void closeRequestSignal(ShaderLab::Shader);
    bool save(ShaderLab::Shader);
    bool saveAs(ShaderLab::Shader);
    void textChanged(ShaderLab::Shader);
    void activateStatusChanged(ShaderLab::Shader, bool v);
    //
    void newShader();
    void saveShader();
    void saveShaderAs();
    void openShader();
    void tabCloseRequested(int tabIdx);
    void run();
    //
    void projectOpened(Project*);
};

#endif // SLSHADERCONTROLLER_H
