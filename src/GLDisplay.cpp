#include <QDebug>
#include <QToolTip>
#include "GLDisplay.h"

using namespace ShaderLab;

GLDisplay::GLDisplay(QWidget *parent) : QGLWidget(parent),
    rigthPressedPoint(NULLPOINT),
    leftPressedPoint(NULLPOINT), wireFrame(false)
{
}

GLDisplay::~GLDisplay()
{
}

void GLDisplay::initializeGL()
{
    glEnable(GL_TEXTURE_2D);
    glEnable( GL_LIGHT0 );
    glEnable(GL_LIGHTING);
    glShadeModel( GL_SMOOTH );

    glClearColor( 0.0, 0.0, 0.5, 1.0);

    GLfloat amb_light[4] = { 1, 1, 1, 1.0 };
    GLfloat ligthPosition[4] = { 0.0, 20.0, -3.0, 1.0 };

    glLightfv( GL_LIGHT0, GL_AMBIENT, amb_light);
    glLightfv( GL_LIGHT0, GL_POSITION, ligthPosition);
}

void GLDisplay::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();
    gluPerspective(45, width*1.0/height, 2.5, 500);
    glMatrixMode(GL_MODELVIEW);
}

void GLDisplay::paintGL()
{
    glClear(GL_COLOR_BUFFER_BIT);

    glColor3f(1.0, 1.0, 1.0);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0,0,-4, 0,0,0, 0,-1,0);

    if(wireFrame)
    {
        glDisable(GL_CULL_FACE);
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glEnable(GL_CULL_FACE);
        glCullFace(GL_BACK);
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    emit drawModel();

    glFlush();
}

void GLDisplay::setWireframe(bool wireframe)
{
    wireFrame = wireframe;
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
            //emit mouseRigthFinish(rigthPressedPoint, event->pos());
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
            //emit mouseRigthMove(rigthPressedPoint, event->pos());
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
