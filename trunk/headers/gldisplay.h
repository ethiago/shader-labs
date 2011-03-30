#ifndef GLDISPLAY_H
#define GLDISPLAY_H

#include <QGLWidget>

namespace ShaderLab{

class GLDisplay : public QGLWidget
{
    Q_OBJECT

public:
    GLDisplay(QWidget *parent);

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();
};

}
#endif // GLDISPLAY_H
