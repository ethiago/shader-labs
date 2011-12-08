#ifndef PRIMITIVESDIALOG_H
#define PRIMITIVESDIALOG_H

#include <QDialog>

namespace Ui {
    class PrimitivesDialog;
}

class PrimitivesDialog : public QDialog {
    Q_OBJECT
public:
    PrimitivesDialog(QStringList ouTexts, QWidget *parent = 0);
    ~PrimitivesDialog();
    int getCurrentInputPrimitiveIndex(void);
    int getCurrentOutputPrimitiveIndex(void);
    void changeCurrentInputType(const QString& s);

protected:
    void changeEvent(QEvent *e);

signals:

private:
    Ui::PrimitivesDialog *ui;
};

#endif // PRIMITIVESDIALOG_H
