#ifndef PRIMITIVESDIALOG_H
#define PRIMITIVESDIALOG_H

#include <QtOpenGL>
#include <QDialog>

namespace Ui {
    class PrimitivesDialog;
}

class PrimitivesDialog : public QDialog {
    Q_OBJECT

    Ui::PrimitivesDialog *ui;

public:
    PrimitivesDialog(QWidget *parent = 0);
    ~PrimitivesDialog();
    //int getCurrentInputPrimitiveIndex(void);
    int getCurrentOutputPrimitiveIndex(void);
    //void changeCurrentInputType(const QString& s);
    GLenum getCurrentOutputPrimitive(void);
    //GLenum getCurrentInputPrimitive(void);

protected:
    void changeEvent(QEvent *e);

signals:

private:
    void primitiveSetup();


};

#endif // PRIMITIVESDIALOG_H
