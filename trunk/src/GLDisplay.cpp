#include "GLDisplay.h"
#include <QDebug>
#include <QToolTip>
#include <QMatrix4x4>
#include "Global.h"

#define ZOOMSTEP 0.05

GLDisplay::GLDisplay(QGLContext* context, QWidget *parent) :
    QGLWidget(context,parent),
    rigthPressedPoint(NULLPOINT),
    leftPressedPoint(NULLPOINT), zoom(1.0)
{
    setStyleSheet("border: 2px solid black;");

    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    setupPropertiesList();
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
    ShaderLab *sl = ShaderLab::instance();
    sl->setContext(this);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable(GL_TEXTURE_3D);
    glEnable( GL_LIGHT0 );
    glEnable(GL_LIGHTING);
    glShadeModel( GL_SMOOTH );
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glEnable(GL_BLEND);
    glBlendFunc(GL_SRC_ALPHA, GL_ONE_MINUS_SRC_ALPHA);

    glHint( GL_PERSPECTIVE_CORRECTION_HINT, GL_NICEST );

    glClearDepth( 1.0 );
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
    float aspect = float(width())/height();

    float xdist = xDist(aspect);
    float ydist = yDist(aspect);

    emit lightSetup();

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    bool o = properties.find(Ortho).value()->value().value<bool>();

    if(o)
        glOrtho(-xdist*3.0, xdist*3.0, -ydist*3.0, +ydist*3.0, 5.0, 50);
    else
        glFrustum(-xdist, xdist, -ydist, +ydist, 5.0, 50);

    glMatrixMode(GL_MODELVIEW);

    QColor c = properties.find(BackGroundColor).value()->value().value<QColor>();
    glClearColor( c.redF(), c.greenF(), c.blueF(), c.alphaF());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0,0,-15, 0,0,0, 0,-1,0);

    c = properties.find(ModelColor).value()->value().value<QColor>();
    glColor4f( c.redF(), c.greenF(), c.blueF(), c.alphaF());

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

void GLDisplay::setupPropertiesList()
{
    variantManager = new QtVariantPropertyManager();

    topItem = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(),
                QLatin1String("Global Property"));

    {
        QColor c = QColor(150, 190, 190);
        item = variantManager->addProperty(QVariant::Color,
                                           QLatin1String("BackGround Color"));
        item->setValue(c);
        topItem->addSubProperty(item);
        properties.insert(BackGroundColor, item);
    }

    {
        QColor c = QColor(255, 255, 255);
        item = variantManager->addProperty(QVariant::Color,
                                           QLatin1String("Model Color"));
        item->setValue(c);
        topItem->addSubProperty(item);
        properties.insert(ModelColor, item);
    }

    {
        item = variantManager->addProperty(QVariant::Bool,
                                           QLatin1String("Ortho"));
        item->setValue(false);
        topItem->addSubProperty(item);
        properties.insert(Ortho, item);
    }

    variantFactory = new QtVariantEditorFactory();

    variantEditor = new QtTreePropertyBrowser();
    variantEditor->setFactoryForManager(variantManager, variantFactory);
    variantEditor->addProperty(topItem);
    variantEditor->setPropertiesWithoutValueMarked(true);
    variantEditor->setRootIsDecorated(false);

    connect(variantEditor, SIGNAL(currentItemChanged(QtBrowserItem*)),
            this, SLOT(attributeChanged(QtBrowserItem*)));
}

void GLDisplay::wheelEvent(QWheelEvent * event)
{
    int nsteps = event->delta() / (8*15);

    if (event->orientation() == Qt::Vertical) {
        zoom += ZOOMSTEP * nsteps;
        event->accept();
        updateGL();
    }else event->ignore();
 }

QtTreePropertyBrowser* GLDisplay::getPropertyBrowser()
{
    return variantEditor;
}

void GLDisplay::attributeChanged(QtBrowserItem *)
{
    updateGL();
}
