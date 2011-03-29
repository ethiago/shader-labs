#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <codechoicedialog.h>

namespace Ui {
    class MainWindow;
}

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void openDialog(void);
    void selectedShader(ShaderLab::Shader);
    void vertexFileSelected(const QString&);
    void fragmentFileSelected(const QString&);
    void closeTabRequest(int index);

signals:
    void selectedFile(const QString& , ShaderLab::Shader);
    void closeTabRequest(ShaderLab::Shader);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setVisibleVertexTab(bool);
    void setVisibleFragmentTab(bool);
    void setVertexCode(const QString &);
    void setFragmentCode(const QString &);

    bool visibleVertexTab(void);
    bool visibleFragmentTab(void);
    QString vertexCode(void);
    QString fragmentCode(void);

private:
    Ui::MainWindow *ui;

    QFileDialog *openFile;
    CodeChoiceDialog *choiceDialog;

};

#endif // MAINWINDOW_H
