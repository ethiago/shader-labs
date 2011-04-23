#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QMap>

#include "shadercodecontainer.h"
#include "codechoicedialog.h"
#include "gldisplay.h"

namespace Ui {
    class MainWindow;
}

using namespace ShaderLab;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void exitApplication(void);
    void openDialog(void);
    void selectedShaderOpenDialog(ShaderLab::Shader);
    void newDialog(void);
    void selectedShaderNewDialog(ShaderLab::Shader);
    void closeTabRequest(int index);
    void runShadersSelected(void);
    void viewMenuClicked(QAction*);
    void dockOutputVisibilityChange(bool);
    void dockRenderVisibilityChange(bool);
    void addShader(ShaderLab::Shader shadertype);
    void saveFile(void);
    void textChanged(ShaderLab::Shader);
    void saveFileAsDialog(void);

signals:
    void selectedFile(const QString& , ShaderLab::Shader);
    void newShaderFile(ShaderLab::Shader);
    void runShaders(void);
    void closeTabRequest(ShaderLab::Shader);
    void programClose();
    void saveFile(ShaderLab::Shader);
    void saveFileAs(ShaderLab::Shader, const QString&, const QString&);
    void shaderCodeChanged(ShaderLab::Shader);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    bool setVisibleShader(bool, ShaderLab::Shader);
    bool setShaderCode(const QString&, ShaderLab::Shader);

    bool visibleShader(ShaderLab::Shader);
    QString shaderCode(ShaderLab::Shader);

    bool updateDisplay();

    bool setOutputText(const QString& );
    QString getOutputText(void);

    void setFileNameDisplay(QString filename, bool changed, ShaderLab::Shader);

    bool saveRequest(const QString&);
    QString saveAsRequest(ShaderLab::Shader);

private:
    Ui::MainWindow *ui;

    CodeChoiceDialog *choiceDialog;
    GLDisplay *display;
    QTabWidget *tabArea;

    QMap<ShaderLab::Shader, ShaderCodeContainer*> codeTabs;

protected:
    void closeEvent(QCloseEvent *event);

};

#endif // MAINWINDOW_H
