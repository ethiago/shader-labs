#ifndef GLDISPLAY_H
#define GLDISPLAY_H

//#if defined(__WIN32__)
//#include <GLee.h>
//#endif

#include <QGLWidget>
#include <QMouseEvent>
#include <QMoveEvent>
#include <QPoint>
#include <QMap>
#include <QtProperty>
#include <QtVariantProperty>
#include <QtTreePropertyBrowser>
#include <QTimer>

#define NULLPOINT   QPoint(-1,-1)
#define EPSILON     0.001

class GLDisplay : public QGLWidget
{
    Q_OBJECT

    QTimer timer;
    QPoint rigthPressedPoint;
    QPoint leftPressedPoint;
    float zoom;
    float frustumZoom;

    enum Properties{
        BackGroundColor,
        Ortho,
        Continuous,
        FrustumZoom,
        BackfaceCulling
    };

signals:
    void drawModel(void);
    void lightSetup(void);
    void mouseRigthMove(QPoint ini, QPoint curr);
    void mouseRigthFinish(QPoint ini, QPoint curr);
    void mouseLeftMove(QPoint ini, QPoint curr);
    void mouseLefthFinish(QPoint ini, QPoint curr);
    void mouseCancel();

public:
    GLDisplay(QGLContext* context,QWidget *parent = 0);
    ~GLDisplay();

    void initializeGL();
    void resizeGL(int w, int h);
    void paintGL();

    QtTreePropertyBrowser* getPropertyBrowser();

private slots:
    void attributeChanged(QtBrowserItem *);
    void valueChanged(QtProperty*,QVariant);
    void timeout();

private:

    QtVariantPropertyManager *variantManager;
    QtProperty *topItem;
    QtVariantProperty *item;
    QtVariantEditorFactory *variantFactory;
    QtTreePropertyBrowser *variantEditor;

    QMap<Properties, QtVariantProperty*> properties;

    void mousePressEvent ( QMouseEvent * event );
    void mouseReleaseEvent ( QMouseEvent * event );
    void mouseMoveEvent(QMouseEvent *);
    void wheelEvent(QWheelEvent *);
    float xDist(float aspect);
    float yDist(float aspect);
    void setupPropertiesList();

};


#endif // GLDISPLAY_H
