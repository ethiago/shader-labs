#ifndef CODECHOICEDIALOG_H
#define CODECHOICEDIALOG_H

#include <QDialog>
#include <QMap>
#include "commandlinkbutton.h"

#include "global.h"

namespace Ui{
    class Dialog;
}

class CodeChoiceDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CodeChoiceDialog(QWidget *parent = 0);
    ~CodeChoiceDialog();
    void addButton(ShaderLab::Shader shadertype);

signals:
    void shader(ShaderLab::Shader);

public slots:
    void arebaba(ShaderLab::Shader shadertype);

private:
    Ui::Dialog *ui;
    QMap<ShaderLab::Shader, CommandLinkButton*> buttons;

};



#endif // CODECHOICEDIALOG_H
