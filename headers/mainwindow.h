#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <codechoicedialog.h>
#include "gldisplay.h"
#include <QVBoxLayout>

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
    void vertexFileSelected(const QString&);
    void fragmentFileSelected(const QString&);
    void closeTabRequest(int index);
    void runShadersSelected(void);

signals:
    void selectedFile(const QString& , ShaderLab::Shader);
    void runShaders(void);
    void closeTabRequest(ShaderLab::Shader);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setVisibleShader(bool, ShaderLab::Shader);
    void setShaderCode(const QString&, ShaderLab::Shader);

    void setVisibleVertexTab(bool);
    void setVisibleFragmentTab(bool);
    void setVertexCode(const QString &);
    void setFragmentCode(const QString &);

    bool visibleShader(ShaderLab::Shader);
    QString shaderCode(ShaderLab::Shader);

    bool visibleVertexTab(void);
    bool visibleFragmentTab(void);
    QString vertexCode(void);
    QString fragmentCode(void);

    void updateDisplay();

private:
    Ui::MainWindow *ui;
  //  QVBoxLayout *glDisplayLayout;

    QFileDialog *openFile;
    CodeChoiceDialog *choiceDialog;

    GLDisplay *display;

};

#endif // MAINWINDOW_H
