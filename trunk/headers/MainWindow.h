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
#include "ChooseShaderDialog.h"

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
    void closeTabRequest(int index);
    void dockOutputVisibilityChange(bool);
    void dockRenderVisibilityChange(bool);
    void exitApplication(void);
    void newDialog(void);
    void openDialog(void);
    void runSelectedShaders(void);
    void saveFile(void);
    void saveFileAsDialog(void);
    void selectedShaderNewDialog(ShaderLab::Shader);
    void selectedShaderOpenDialog(ShaderLab::Shader);
    void textChanged(ShaderLab::Shader);
    void viewMenuClicked(QAction*);
    void changeActivationStatus(void);

/* Created by the UI */
signals:
    void closeTabRequest(ShaderLab::Shader);
    void newShaderFile(ShaderLab::Shader);
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

    void addShader(ShaderLab::Shader shadertype);
    QString getOutputText(void);
    QString saveAsRequest(ShaderLab::Shader);
    void setFileNameDisplay(QString filename, bool changed, ShaderLab::Shader);
    bool setOutputText(const QString& );
    bool setShaderCode(const QString&, ShaderLab::Shader);
    bool setVisibleShader(bool, ShaderLab::Shader);
    QString shaderCode(ShaderLab::Shader);
    bool visibleShader(ShaderLab::Shader);
    void setGLDisplay(GLDisplay *);
    void setEnableShaderCode(ShaderLab::Shader, bool);
    QAction* actionTexturePropertiesView(void);
    QMenu* modelsMenu(void);

private:
    Ui::MainWindow *ui;   /* The Main window. */

    QMap<ShaderLab::Shader, ShaderCodeContainer*> codeTabs; /* Tab objects for each kind of shader. */
    SLTabWidget *tabArea;  /* UI component for nesting tabs. */

protected:
    void closeEvent(QCloseEvent *event);

};

#endif // MAINWINDOW_H
