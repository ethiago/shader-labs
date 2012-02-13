#ifndef SLCODECONTAINER_H
#define SLCODECONTAINER_H

#include <QWidget>
#include <QTimer>
#include <QShortcut>
#include "Global.h"

namespace Ui {
    class ShaderCodeContainer;
}

class Highlighter;

class SLCodeContainer : public QWidget
{
    Q_OBJECT

    QShortcut * m_save;
    bool m_active;
public:
    explicit SLCodeContainer(ShaderLab::Shader shadertype, QWidget *parent = 0);
    ~SLCodeContainer();
    void setText(const QString&);
    QString getText(void);
    ShaderLab::Shader getShaderType(void);
    void setActivatedCode(bool);
    bool activateCode();
    void changeActivatedStatus();

    void findNext(const QString&);
    void findBack(const QString&);
    void replaceNext(const QString&, const QString&);
    void replaceAll(const QString&, const QString&);
    void closeRequest();
    void close();
    void changeActivationStatus();

private:
    Ui::ShaderCodeContainer *ui;
    QPalette activePalette;
    QPalette inactivePalette;
    ShaderLab::Shader shaderType;
    Highlighter * highLighter;

    QPoint getCursorPosition(const QString& text, int pos);

private slots:
    void textChanged(bool);
    void cursorPositionChanged(void);

public slots:
    void setFocus();

signals:
    void textChanged(ShaderLab::Shader);
    void textChanged();
    void clicked(Qt::MouseButton, ShaderLab::Shader);
    void doubleClicked(ShaderLab::Shader);
    void closeSignal(QWidget*);
    void closeRequestSignal();
    void save();
    void activateStatusChanged();

protected:
    void mouseReleaseEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);

};

#endif // SLCODECONTAINER_H
