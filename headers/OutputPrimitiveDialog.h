#ifndef OUTPUTPRIMITIVEDIALOG_H
#define OUTPUTPRIMITIVEDIALOG_H

#include <QDialog>

namespace Ui {
    class OutputPrimitiveDialog;
}

class OutputPrimitiveDialog : public QDialog {
    Q_OBJECT
public:
    OutputPrimitiveDialog(QWidget *parent = 0);
    ~OutputPrimitiveDialog();

protected:
    void changeEvent(QEvent *e);

private:
    Ui::OutputPrimitiveDialog *ui;
};

#endif // OUTPUTPRIMITIVEDIALOG_H
