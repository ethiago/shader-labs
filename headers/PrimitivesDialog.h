#ifndef PRIMITIVESDIALOG_H
#define PRIMITIVESDIALOG_H

#include <QDialog>
#include "gl3w.h"

namespace Ui {
    class PrimitivesDialog;
}

class PrimitivesDialog : public QDialog {
    Q_OBJECT

    Ui::PrimitivesDialog *ui;

public:
    PrimitivesDialog(QWidget *parent = 0);
    ~PrimitivesDialog();

    int getCurrentOutputPrimitiveIndex(void);
    GLenum getCurrentOutputPrimitive(void);
    GLuint getOutputVertexCount(void);
    void setMaxOutputVertexOut(int);

signals:
    void valueChanged();

private:
    void primitiveSetup();


};

#endif // PRIMITIVESDIALOG_H
