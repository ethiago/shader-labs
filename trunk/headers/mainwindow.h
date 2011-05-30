#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QMap>

#include "shadercodecontainer.h"
#include "codechoicedialog.h"

namespace Ui {
    class MainWindow;
}

namespace ShaderLab
{
    class GLDisplay;
}

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

/* Created by the UI */
signals:
    void closeTabRequest(ShaderLab::Shader);
    void newShaderFile(ShaderLab::Shader);
    void programClose();
    void runShaders(void);
    void saveAll(void);
    void saveFile(ShaderLab::Shader);
    void saveFileAs(ShaderLab::Shader, const QString&, const QString&);
    void selectedFile(const QString& , ShaderLab::Shader);
    void shaderCodeChanged(ShaderLab::Shader);
    void drawModel(void);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void addShader(ShaderLab::Shader shadertype);
    QString getOutputText(void);
    QString saveAsRequest(ShaderLab::Shader);
    bool saveRequest(const QString&, bool newFile = false);
    void setFileNameDisplay(QString filename, bool changed, ShaderLab::Shader);
    bool setOutputText(const QString& );
    bool setShaderCode(const QString&, ShaderLab::Shader);
    bool setVisibleShader(bool, ShaderLab::Shader);
    QString shaderCode(ShaderLab::Shader);
    bool visibleShader(ShaderLab::Shader);
    void setGLDisplay(GLDisplay *);

private:
    Ui::MainWindow *ui;   /* The Main window. */

    QMap<ShaderLab::Shader, ShaderCodeContainer*> codeTabs; /* Tab objects for each kind of shader. */
    QTabWidget *tabArea;  /* UI component for nesting tabs. */

    CodeChoiceDialog *choiceDialog;  /* The choice dialog. */

protected:
    void closeEvent(QCloseEvent *event);

};

#endif // MAINWINDOW_H
