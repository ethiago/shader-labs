#ifndef GLDISPLAY_H
#define GLDISPLAY_H

#include <QGLWidget>
#include <QMouseEvent>
#include <QMoveEvent>
#include <QPoint>

#define NULLPOINT    QPoint(-1,-1)

namespace ShaderLab{

class GLDisplay : public QGLWidget
{
    Q_OBJECT

    QPoint rigthPressedPoint;
    QPoint leftPressedPoint;
    bool wireFrame;

signals:
    void drawModel(void);
    void mouseRigthMove(QPoint ini, QPoint curr);
    void mouseRigthFinish(QPoint ini, QPoint curr);
    void mouseLeftMove(QPoint ini, QPoint curr);
    void mouseLefthFinish(QPoint ini, QPoint curr);
    void mouseCancel();

public:
    GLDisplay(QWidget *parent = 0);
    ~GLDisplay();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    void setWireframe(bool);

private:
    void mousePressEvent ( QMouseEvent * event );
    void mouseReleaseEvent ( QMouseEvent * event );
    void mouseMoveEvent(QMouseEvent *);

};

}
#endif // GLDISPLAY_H
