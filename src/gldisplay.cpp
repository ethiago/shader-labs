#include "gldisplay.h"

using namespace ShaderLab;

GLDisplay::GLDisplay(QWidget *parent) : QGLWidget(parent)
{
}

void GLDisplay::initializeGL()
{
     glEnable(GL_DEPTH_TEST);
     glEnable(GL_CULL_FACE);
     glClearColor(0.0, 0.0, 0.0, 0.0);
     glEnable(GL_DEPTH_TEST);     
 }

 void GLDisplay::resizeGL(int width, int height)
 {

     int side = qMin(width, height);
     glViewport((width - side) / 2, (height - side) / 2, side, side);

     glMatrixMode(GL_PROJECTION);
     glLoadIdentity();

     glOrtho(-0.5, +0.5, +0.5, -0.5, 4.0, 15.0);

     glMatrixMode(GL_MODELVIEW);
 }

 void GLDisplay::paintGL()
 {
    glLoadIdentity();

    GLUquadric *quad = gluNewQuadric();

    glColor3f(255,255,255);
    gluSphere(quad, 1.0, 10, 10);
 }
