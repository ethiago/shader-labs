#ifndef CHOOSESHADERDIALOG_H
#define CHOOSESHADERDIALOG_H

#include <QDialog>
#include <QMap>
#include "SLCommandLinkButton.h"

#include "Global.h"

namespace Ui{
    class Dialog;
}

class ChooseShaderDialog : public QDialog
{
    Q_OBJECT
public:
    explicit ChooseShaderDialog(QWidget *parent = 0);
    ~ChooseShaderDialog();
    void addButton(ShaderLab::Shader shadertype);

signals:
    void shader(ShaderLab::Shader);

public slots:
    void arebaba(ShaderLab::Shader shadertype);

private:
    Ui::Dialog *ui;
    QMap<ShaderLab::Shader, SLCommandLinkButton*> buttons;

};



#endif // CHOOSESHADERDIALOG_H
