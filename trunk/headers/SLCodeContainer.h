#ifndef SLCODECONTAINER_H
#define SLCODECONTAINER_H

#include <QWidget>
#include <QTimer>
#include <QShortcut>
#include <QPlainTextEdit>
#include "Global.h"

namespace Ui {
    class ShaderCodeContainer;
}

class Highlighter;
class CodeEditor;

class SLCodeContainer : public QWidget
{
    Q_OBJECT

    QShortcut * m_save;
    bool m_active;
public:
    explicit SLCodeContainer(ShaderLab::Shader shadertype, const QString& title = QString(), QWidget *parent = 0);
    ~SLCodeContainer();
    void setText(const QString&);
    QString getText(void);
    ShaderLab::Shader getShaderType(void);
    bool activateCode();
    void changeActivatedStatus();

    void findNext(const QString&);
    void findBack(const QString&);
    void replaceNext(const QString&, const QString&);
    void replaceAll(const QString&, const QString&);
    void closeRequest();
    void close();
    void changeActivationStatus();
    void setTabTitle(const QString& title);
    void setTabIcon(const QIcon& icon);
    void updateTabBar();
    void saveShader();
    void saveShaderAs();

private:
    Ui::ShaderCodeContainer *ui;
    CodeEditor *shaderCodeBox;
    QPalette activePalette;
    QPalette inactivePalette;
    ShaderLab::Shader shaderType;
    Highlighter * highLighter;

    QPoint getCursorPosition(const QString& text, int pos);
    void setupCodeBox();

private slots:
    void cursorPositionChanged(void);

public slots:
    void setFocus();
    void textChanged(bool);

signals:
    void textChanged(ShaderLab::Shader);
    void textChanged();
    void clicked(Qt::MouseButton, ShaderLab::Shader);
    void doubleClicked(ShaderLab::Shader);
    void closeSignal(QWidget*);
    void closeRequestSignal();
    void save();
    void activateStatusChanged();
    void setTabTitle(const QString& title, QWidget*);
    void setTabIcon(const QIcon& icon, QWidget*);
    void saveAs();

protected:
    void mouseReleaseEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);

};

#endif // SLCODECONTAINER_H