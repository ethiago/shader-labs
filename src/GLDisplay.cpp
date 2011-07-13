#include <QDebug>
#include <QToolTip>
#include "GLDisplay.h"
#include <QMatrix4x4>

#define MINI(a,b) ((a) < (b) ? (a) : (b))
using namespace ShaderLab;

GLDisplay::GLDisplay(QWidget *parent) : QGLWidget(parent),
    rigthPressedPoint(NULLPOINT),
    leftPressedPoint(NULLPOINT), wireFrame(false), zoom(1.0)
{
    setStyleSheet("border: 2px solid black;");
}

GLDisplay::~GLDisplay()
{
}

void GLDisplay::setZoom(float z)
{
    zoom = z;
}

float GLDisplay::getZoom(void)const
{
    return zoom;
}

void GLDisplay::initializeGL()
{
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable( GL_LIGHT0 );
    glEnable(GL_LIGHTING);
    glShadeModel( GL_SMOOTH );

    glClearColor( 0.0, 0.0, 0.5, 1.0);

    GLfloat ligthPosition[4] = { 0.0, 20.0, -3.0, 1.0 };

    glLightfv( GL_LIGHT0, GL_POSITION, ligthPosition);

    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );	// specify implementation-specific hints

    glClearDepth( 1.0 );					// specify the clear value for the depth buffer
    glDepthFunc( GL_LEQUAL );
    glDisable(GL_CULL_FACE);
}

void GLDisplay::resizeGL(int width, int height)
{
    glViewport(0, 0, width, height);
}

float GLDisplay::xDist(float aspect)
{
    if(aspect < 1)
        return (0.5)*zoom;
    else
        return (0.5*aspect)*zoom;
}

float GLDisplay::yDist(float aspect)
{
    if(aspect > 1)
        return (0.5)*zoom;
    else
        return (0.5*(1/aspect))*zoom;
}

void GLDisplay::paintGL()
{
    float aspect = width()*1.0/height();

    float xdist = xDist(aspect);
    float ydist = yDist(aspect);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    glFrustum(-xdist, xdist, -ydist, +ydist, 5.0, 100);

    glMatrixMode(GL_MODELVIEW);

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0,0,-15, 0,0,0, 0,-1,0);

    if(wireFrame)
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE);
    }
    else
    {
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL);
    }

    glColor3f(1.0, 1.0, 1.0);

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

