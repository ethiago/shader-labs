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
    void selectedShader(ShaderLab::Shader);
    void closeTabRequest(int index);
    void runShadersSelected(void);
    void viewMenuClicked(QAction*);
    void dockOutputVisibilityChange(bool);
    void dockRenderVisibilityChange(bool);
    void addShader(ShaderLab::Shader shadertype);

signals:
    void selectedFile(const QString& , ShaderLab::Shader);
    void runShaders(void);
    void closeTabRequest(ShaderLab::Shader);
    void programClose();

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
