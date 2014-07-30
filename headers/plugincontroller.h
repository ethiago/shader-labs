#ifndef PLUGINCONTROLLER_H
#define PLUGINCONTROLLER_H

#include <QObject>
#include <QMenu>
#include <QAction>
#include <QDir>
#include <QSet>
#include <QMap>

#include "PerVertexInterface.h"
#include "plydataitens.h"

class SLObject;

typedef struct _PLUGINPV
{
    PerVertexInterface * pluginObject;
    QString pluginName;
}PLUGINPV;

class PluginController : public QObject
{
    Q_OBJECT

    SLObject * e_object;
    QMenu * e_pluginMenu;

    QAction * e_updatePluginList;

    QMenu * e_perVertexPluginsMenu;
    QMap<QAction*, PLUGINPV> m_perVertexPlugins;

    QDir pluginsDir;

public:
    explicit PluginController(QMenu* pluginMenu, SLObject* obj);
    ~PluginController();

    void changeObject(SLObject * object);
    
signals:
    void pluginRunned(void);

public slots:
    void updatePluginList();
    void perVertexAction(QAction* act);

private:
    void populatePluginMenu(QObject *plugin, const QString& fn);
    static PLYDataHeader::Property toProperty(const QPair<QString, DataType>& pair);
    static QVector3D toGeometry(const QList<QVariant>& list);
    static QVector2D toUV(const QList<QVariant>& list);

    
};

#endif // PLUGINCONTROLLER_H
