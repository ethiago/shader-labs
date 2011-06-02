#include <QDebug>
#include <QToolTip>
#include "gldisplay.h"

using namespace ShaderLab;

GLDisplay::GLDisplay(QWidget *parent) : QGLWidget(parent),
    rigthPressedPoint(NULLPOINT),
    leftPressedPoint(NULLPOINT)
{
}

GLDisplay::~GLDisplay()
{
}

void GLDisplay::initializeGL()
{
}

void GLDisplay::resizeGL(int width, int height)
{
    int side = qMin(width, height);
    glViewport((width - side) / 2, (height - side) / 2, side, side);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, 1.0, 2.0, 500);
    glMatrixMode(GL_MODELVIEW);
}

void GLDisplay::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    glColor3f(1.0, 1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0,0,-3, 0,0,0, 0,-1,0);

    emit drawModel();

    glFlush();
}

void GLDisplay::mousePressEvent ( QMouseEvent * event )
{
    // Garante que somente 1 botao estara' pressionado e
    //   cancela a operacao anterior caso o outro seja
    //   pressionado
    if(event->button() == Qt::RightButton)
    {
        if(leftPressedPoint != NULLPOINT)
        {
            leftPressedPoint = NULLPOINT;
            rigthPressedPoint = NULLPOINT;
            emit mouseCancel();
        }else
        {
            rigthPressedPoint = event->pos();
        }
        event->accept();
    }else if(event->button() == Qt::LeftButton)
    {
        if(rigthPressedPoint != NULLPOINT)
        {
            leftPressedPoint = NULLPOINT;
            rigthPressedPoint = NULLPOINT;
            emit mouseCancel();
        }else
        {
            leftPressedPoint = event->pos();
        }
        event->accept();
    }else
        event->ignore();
}

void GLDisplay::mouseReleaseEvent ( QMouseEvent * event )
{
    if(event->button() == Qt::RightButton)
    {
        if(rigthPressedPoint != NULLPOINT)
        {
            emit mouseRigthFinish(rigthPressedPoint, event->pos());
            rigthPressedPoint = NULLPOINT;
        }
        event->accept();
    }else if(event->button() == Qt::LeftButton)
    {
        if(leftPressedPoint != NULLPOINT)
        {
            emit mouseLefthFinish(leftPressedPoint, event->pos());
            leftPressedPoint = NULLPOINT;
        }
    }else
        event->ignore();
}

void GLDisplay::mouseMoveEvent(QMouseEvent *event)
{
    if(event->buttons() == Qt::RightButton)
    {
        if(rigthPressedPoint != NULLPOINT)
        {
            emit mouseRigthMove(rigthPressedPoint, event->pos());
        }
        event->accept();
    }else if(event->buttons() == Qt::LeftButton)
    {
        if(leftPressedPoint != NULLPOINT)
        {
            emit mouseLeftMove(leftPressedPoint, event->pos());
        }
        event->accept();
    }else
        event->ignore();
}
