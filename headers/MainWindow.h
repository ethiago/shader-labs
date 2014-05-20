#ifndef MAINWINDOW_H
#define MAINWINDOW_H

#include <QMainWindow>
#include <QFileDialog>
#include <QVBoxLayout>
#include <QTabWidget>
#include <QMap>
#include <QMenu>
#include <QTabBar>

#include "Find.h"

namespace Ui {
    class MainWindow;
}

class SLTabWidget;
class GLDisplay;

class MainWindow : public QMainWindow
{
    Q_OBJECT

public slots:
    void exitApplication(void);
    void shaderLog(const QString& log);
    void tabVisibilityChanged(bool);

/* Created by the UI */
signals:
    void programClose(QCloseEvent *event);
    void runShaders(void);
    void saveAll(void);
    void wireframeClicked();
    void saveResultAsImage();
    void newShaderActionClicked();
    void openShaderActionClicked();
    void lightRotationToggle(bool);
    void loadProject();
    void closeProject();
    void saveAsProject(void);
    void saveProject(void);
    void newPrimitiveDialog();
    void saveShader();
    void saveShaderAs();
    void newObject();
    void origin(bool);
    void objectsVisibility(bool);

public:
    explicit MainWindow(QWidget *parent = 0);
    ~MainWindow();

    void setGLDisplay(GLDisplay *);
    QMenu* modelsMenu(void);
    void menuViewInsertAction(QAction* act);
    void menuViewRemoveAction(QAction* act);
    QAction *addSettingsMenu(QMenu* menu);

    SLTabWidget* getTabWidget();

public slots:
    void setSecondTitle(const QString& title);

private:
    Ui::MainWindow *ui;   /* The Main window. */
    Find* find;

    SLTabWidget *tabArea;  /* UI component for nesting tabs. */

    void createTabWidget();


protected:
    void closeEvent(QCloseEvent *event);
};

#endif // MAINWINDOW_H
