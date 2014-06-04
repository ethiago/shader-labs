#include "GLDisplay.h"
#include <QDebug>
#include <QToolTip>
#include <QMatrix4x4>
#include "Global.h"
#include "qmath.h"

#ifdef __APPLE__
    #include <OpenGL/glext.h>
    #include <OpenGL/glu.h>
#else
    #include <GL/glext.h>
    #include <GL/glu.h>
#endif

#define ZOOMSTEP 0.05

GLDisplay::GLDisplay(QGLContext* context, QWidget *parent) :
    QGLWidget(context,parent),
    rigthPressedPoint(NULLPOINT),
    leftPressedPoint(NULLPOINT), zoom(0.0), frustumZoom(0.0)
{
    setAutoBufferSwap(true);
    setStyleSheet("border: 2px solid black;");

    setSizePolicy(QSizePolicy::Expanding,QSizePolicy::Expanding);

    setupPropertiesList();

    connect(&timer, SIGNAL(timeout()), this, SLOT(timeout()) );

    timer.setInterval(30);
    timer.start();
}

GLDisplay::~GLDisplay()
{
}

void GLDisplay::initializeGL()
{
    ShaderLab *sl = ShaderLab::instance();
    sl->setContext(this);
    glEnable(GL_VERTEX_PROGRAM_POINT_SIZE);
    glEnable(GL_DEPTH_TEST);
    glEnable(GL_TEXTURE_2D);
    glEnable( GL_LIGHT0 );
    glEnable(GL_LIGHTING);
    glShadeModel( GL_SMOOTH );
    glEnable(GL_COLOR_MATERIAL);
    glColorMaterial(GL_FRONT_AND_BACK, GL_AMBIENT_AND_DIFFUSE);
    glDisable( GL_BLEND );

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
        return (0.5)*expf(frustumZoom);
    else
        return (0.5*aspect)*expf(frustumZoom);
}

float GLDisplay::yDist(float aspect)
{
    if(aspect > 1)
        return (0.5)*expf(frustumZoom);
    else
        return (0.5*(1/aspect))*expf(frustumZoom);
}

void GLDisplay::paintGL()
{
    float aspect = float(width())/height();

    float xdist = xDist(aspect);
    float ydist = yDist(aspect);

    emit lightSetup();

    bool bFCulling = properties.find(BackfaceCulling).value()->value().value<bool>();

    if(bFCulling)
        glEnable(GL_CULL_FACE);
    else
        glDisable(GL_CULL_FACE);

    glMatrixMode(GL_PROJECTION);
    glLoadIdentity();

    bool o = properties.find(Ortho).value()->value().value<bool>();

    float n = properties.find(Near).value()->value().value<float>();
    float f = properties.find(Far).value()->value().value<float>();

    if(o)
        glOrtho(-xdist*3.0, xdist*3.0, -ydist*3.0, +ydist*3.0, n, f);
    else
        glFrustum(-xdist, xdist, -ydist, +ydist, n, f);

    glMatrixMode(GL_MODELVIEW);

    QColor c = properties.find(BackGroundColor).value()->value().value<QColor>();
    glClearColor( c.redF(), c.greenF(), c.blueF(), c.alphaF());

    glClear(GL_COLOR_BUFFER_BIT | GL_DEPTH_BUFFER_BIT);

    emit lightSetup();

    glMatrixMode(GL_MODELVIEW);
    glLoadIdentity();

    gluLookAt(0,0,-15-zoom, 0,0,-zoom, 0,-1,0);

    emit drawModel();
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
        item = variantManager->addProperty(QVariant::Bool,
                                           QLatin1String("Ortho"));
        item->setValue(false);
        topItem->addSubProperty(item);
        properties.insert(Ortho, item);
    }

    {
        item = variantManager->addProperty(QVariant::Bool,
                                           QLatin1String("Continuous"));
        item->setValue(false);
        topItem->addSubProperty(item);
        properties.insert(Continuous, item);
    }

    {
        item = variantManager->addProperty(QVariant::Bool,
                                           QLatin1String("FrustumZoom"));
        item->setValue(true);
        topItem->addSubProperty(item);
        properties.insert(FrustumZoom, item);
    }

    {
        item = variantManager->addProperty(QVariant::Bool,
                                           QLatin1String("BackfaceCulling"));
        item->setValue(false);
        topItem->addSubProperty(item);
        properties.insert(BackfaceCulling, item);
    }

    {
        item = variantManager->addProperty(QVariant::Double,
                                           QLatin1String("Near"));
        item->setValue(5.0);
        topItem->addSubProperty(item);
        properties.insert(Near, item);
    }

    {
        item = variantManager->addProperty(QVariant::Double,
                                           QLatin1String("Far"));
        item->setValue(100.0);
        topItem->addSubProperty(item);
        properties.insert(Far, item);
    }

    variantFactory = new QtVariantEditorFactory();

    variantEditor = new QtTreePropertyBrowser();
    variantEditor->setFactoryForManager(variantManager, variantFactory);
    variantEditor->addProperty(topItem);
    variantEditor->setPropertiesWithoutValueMarked(true);
    variantEditor->setRootIsDecorated(false);

    connect(variantEditor, SIGNAL(currentItemChanged(QtBrowserItem*)),
            this, SLOT(attributeChanged(QtBrowserItem*)));

    connect(variantManager, SIGNAL(valueChanged(QtProperty*,QVariant)),
            this, SLOT(valueChanged(QtProperty*,QVariant)) );
}

void GLDisplay::wheelEvent(QWheelEvent * event)
{
    int nsteps = event->delta() / (8*15);

    if (event->orientation() == Qt::Vertical) {
        bool f = properties.find(FrustumZoom).value()->value().value<bool>();
        if(f)
            frustumZoom += ZOOMSTEP * nsteps;
        else
            zoom +=  nsteps/2.0;

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
void GLDisplay::valueChanged(QtProperty*,QVariant)
{
    updateGL();
}

void GLDisplay::timeout()
{
    bool c = properties.find(Continuous).value()->value().value<bool>();
    if(c)
    {
        updateGL();
        ShaderLab * sl = ShaderLab::instance();
        sl->incTime();
    }
}
