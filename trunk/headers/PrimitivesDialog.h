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
    QAction *m_toogleView;

public:
    PrimitivesDialog();
    ~PrimitivesDialog();

    int getCurrentOutputPrimitiveIndex(void);
    GLenum getCurrentOutputPrimitive(void);
    GLuint getOutputVertexCount(void);
    void setMaxOutputVertexOut(int);
    QAction * toogleViewAction();

signals:
    void valueChanged();

public slots:
    void viewAction(bool);

private:
    void primitiveSetup();


protected:
    void showEvent(QShowEvent *evnt);
    void hideEvent(QHideEvent *);


};

#endif // PRIMITIVESDIALOG_H
