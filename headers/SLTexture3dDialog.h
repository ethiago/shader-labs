#ifndef SLTEXTURE3DDIALOG_H
#define SLTEXTURE3DDIALOG_H

#include <QDialog>

namespace Ui {
    class SLTexture3DDialog;
}

class SLTexture3DDialog : public QDialog
{
    Q_OBJECT

public:
    explicit SLTexture3DDialog(QWidget *parent = 0);
    ~SLTexture3DDialog();

    void setFileName(const QString& fn);

    int getFileWidth();
    int getFileHeight();
    int getFileDepth();
    int getLengthData();

private:
    Ui::SLTexture3DDialog *ui;
};

#endif // SLTEXTURE3DDIALOG_H
