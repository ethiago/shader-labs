#ifndef GLDISPLAY_H
#define GLDISPLAY_H

#include <QGLWidget>
#include <QMouseEvent>
#include <QMoveEvent>
#include <QPoint>
#include <QMap>
#include <QtProperty>
#include <QtVariantProperty>
#include <QtTreePropertyBrowser>

#define NULLPOINT   QPoint(-1,-1)
#define EPSILON     0.001

class GLDisplay : public QGLWidget
{
    Q_OBJECT

    QPoint rigthPressedPoint;
    QPoint leftPressedPoint;
    float zoom;

    enum Properties{
        BackGroundColor,
        ModelColor,
        Ortho
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

    void setZoom(float z);
    float getZoom(void)const;

    QtTreePropertyBrowser* getPropertyBrowser();

private slots:
    void attributeChanged(QtBrowserItem *);

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
    float xDist(float aspect);
    float yDist(float aspect);
    void setupPropertiesList();

};


#endif // GLDISPLAY_H
