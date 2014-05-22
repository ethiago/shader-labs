#ifndef SLCODECONTAINER2_H
#define SLCODECONTAINER2_H

#include <QWidget>
#include <QTimer>
#include <QShortcut>
#include <QPlainTextEdit>
#include "Global.h"

namespace Ui {
    class ShaderCodeContainer2;
}

class Highlighter;
class CodeEditor;
class SLTabWidget;

class SLCodeContainer2 : public QWidget
{
    Q_OBJECT

    bool m_active;
    ShaderLab::Shader m_shaderType;
    CodeEditor *shaderCodeBox;

    bool m_disableNextEvent;

public:
    explicit SLCodeContainer2(ShaderLab::Shader shadertype, SLTabWidget *parent, const QString& title = QString());
    ~SLCodeContainer2();
    void setText(const QString&, bool disableNextEvent = true);
    QString getText(void);
    ShaderLab::Shader shaderType(void);

    void changeActivatedStatus();

    void findNext(const QString&);
    void findBack(const QString&);
    void replaceNext(const QString&, const QString&);
    void replaceAll(const QString&, const QString&);
    void closeRequest();

    void setTabTitle(const QString& title);
    void setTabIcon(const QIcon& icon);
    void updateTabBar();

    void hideTab();
    void showTab();

    void setActive(bool v);

private:
    Ui::ShaderCodeContainer2 *ui;

    QPalette activePalette;
    QPalette inactivePalette;
    Highlighter * highLighter;
    QShortcut * m_save;
    SLTabWidget *e_parent;


    QPoint getCursorPosition(const QString& text, int pos);
    void setupCodeBox();

private slots:
    void cursorPositionChanged(void);

public slots:
    void setFocus();
    void textChanged(bool);
    void saveFromShotcut();

signals:
    void closeRequestSignal(ShaderLab::Shader);
    void save(ShaderLab::Shader);
    void textChanged(ShaderLab::Shader);
    void activateStatusChanged(ShaderLab::Shader, bool);
    //
    void closeSignal(QWidget*);
    void setTabTitle(const QString& title, QWidget*);
    void setTabIcon(const QIcon& icon, QWidget*);


};

#endif // SLCODECONTAINER2_H
