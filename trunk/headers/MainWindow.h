#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QMap>
#include <QMenu>
#include <QTabBar>

#include "ShaderCodeContainer.h"
//#include "ChooseShaderDialog.h"
#include "Find.h"

namespace Ui {
    class MainWindow;
}

namespace ShaderLab
{
    class GLDisplay;
}

class SLTabWidget;

using namespace ShaderLab;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void exitApplication(void);
    void saveFile(void);
    void saveFileAsDialog(void);

/* Created by the UI */
signals:
    void closeTabRequest(ShaderLab::Shader);
    void programClose(QCloseEvent *event);
    void runShaders(void);
    void saveAll(void);
    void saveFile(ShaderLab::Shader);
    void saveFileAs(ShaderLab::Shader);
    void selectedFile(ShaderLab::Shader);
    void shaderCodeChanged(ShaderLab::Shader);
    void drawModel(void);
    void wireframeClicked(bool);
    void changeActivationStatusClicked(ShaderLab::Shader);
    void saveResultAsImage();
    void newShaderActionClicked();
    void openShaderActionClicked();

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    //QString getOutputText(void);
    void addShader(ShaderLab::Shader shadertype);
    void setFileNameDisplay(QString filename, bool changed, ShaderLab::Shader);
    bool setOutputText(const QString& );
    bool setShaderCode(const QString&, ShaderLab::Shader);
    bool setVisibleShader(bool, ShaderLab::Shader);
    QString shaderCode(ShaderLab::Shader);
    bool visibleShader(ShaderLab::Shader);
    void setGLDisplay(GLDisplay *);
    void setEnableShaderCode(ShaderLab::Shader, bool);
    QMenu* modelsMenu(void);
    void menuViewInsertAction(QAction* act);

private:
    Ui::MainWindow *ui;   /* The Main window. */
    Find* find;

    QMap<ShaderLab::Shader, ShaderCodeContainer*> codeTabs; /* Tab objects for each kind of shader. */
    SLTabWidget *tabArea;  /* UI component for nesting tabs. */

protected:
    void closeEvent(QCloseEvent *event);

};

#endif // MAINWINDOW_H
