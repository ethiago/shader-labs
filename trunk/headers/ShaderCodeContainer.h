#ifndef SHADERCODECONTAINER_H
#define SHADERCODECONTAINER_H

#include <QWidget>
#include <QTimer>
#include "Global.h"

namespace Ui {
    class ShaderCodeContainer;
}

class Highlighter;

class ShaderCodeContainer : public QWidget
{
    Q_OBJECT

public:
    explicit ShaderCodeContainer(ShaderLab::Shader shadertype, QWidget *parent = 0);
    ~ShaderCodeContainer();
    void setText(const QString&);
    QString getText(void);
    ShaderLab::Shader getShaderType(void);
    void setActivatedCode(bool);

    void findNext(const QString&);
    void findBack(const QString&);
    void replaceNext(const QString&, const QString&);
    void replaceAll(const QString&, const QString&);

private:
    Ui::ShaderCodeContainer *ui;
    QPalette activePalette;
    QPalette inactivePalette;
    ShaderLab::Shader shaderType;
    Highlighter * highLighter;

    QPoint getCursorPosition(const QString& text, int pos);

private slots:
    void textChanged(void);
    void cursorPositionChanged(void);

signals:
    void textChanged(ShaderLab::Shader);
    void clicked(Qt::MouseButton, ShaderLab::Shader);
    void doubleClicked(ShaderLab::Shader);

protected:
    void mouseReleaseEvent(QMouseEvent *);
    void mouseDoubleClickEvent(QMouseEvent *);

};

#endif // SHADERCODECONTAINER_H
