#ifndef CODECHOICEDIALOG_H
#define CODECHOICEDIALOG_H

#include <QDialog>
#include "global.h"

namespace Ui{
    class Dialog;
}

class CodeChoiceDialog : public QDialog
{
    Q_OBJECT
public:
    explicit CodeChoiceDialog(QWidget *parent = 0);

signals:
    void shader(ShaderLab::Shader);

public slots:
    void openVertex();
    void openFragment();

private:
    Ui::Dialog *ui;

};



#endif // CODECHOICEDIALOG_H
