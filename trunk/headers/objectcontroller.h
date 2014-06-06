#ifndef OBJECTCONTROLLER_H
#define OBJECTCONTROLLER_H

#include <QObject>
#include <QtProperty>
#include <QtVariantProperty>
#include <QtTreePropertyBrowser>

class VertexProperties;
class MainWindow;
class SLObject;
class AttributeView;

class ObjectController : public QObject
{
    Q_OBJECT

    VertexProperties * m_vertexProperties;
    SLObject * e_object;
    AttributeView * m_attributeView;


public:
    explicit ObjectController(MainWindow * mw, SLObject* obj);
    ~ObjectController();
    void newObject(SLObject *obj);
    void setObject(SLObject *obj);

    
private:

    enum Properties{
        ModelColor,
        Visibible
    };

    QtVariantPropertyManager *variantManager;
    QtProperty *topItem;
    QtVariantProperty *item;
    QtVariantEditorFactory *variantFactory;
    QtTreePropertyBrowser *variantEditor;

    QMap<Properties, QtVariantProperty*> properties;

    void setupPropertiesList();

signals:
    void objectChanged(int);


private slots:
    void attributeChanged(QtBrowserItem*);
    void valueChanged(QtProperty*,QVariant);

public slots:
    void updateView();
    
};

#endif // OBJECTCONTROLLER_H
