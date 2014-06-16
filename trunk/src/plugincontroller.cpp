#include "plugincontroller.h"
#include "SLObject.h"
#include "slhemesh.h"
#include "vertex.h"

#include <QDebug>
#include <QPluginLoader>
#include <QApplication>
#include <QProgressBar>
#include <QDialog>
#include <exception>


PluginController::PluginController(QMenu* pluginMenu, SLObject * obj) :
    QObject(NULL), e_object(obj), e_pluginMenu(pluginMenu)
{
    e_updatePluginList = e_pluginMenu->addAction("&Update List",this, SLOT(updatePluginList()) );
    e_pluginMenu->addSeparator();
    e_perVertexPluginsMenu =  e_pluginMenu->addMenu("Per Vertex Plugins");
    connect(e_perVertexPluginsMenu, SIGNAL(triggered(QAction*)),
            this, SLOT(perVertexAction(QAction*)) );

    pluginsDir = QDir(qApp->applicationDirPath());

    pluginsDir.cd("../plugins");

    updatePluginList();
}

PluginController::~PluginController()
{
}

void PluginController::changeObject(SLObject * object)
{
    e_object = object;
}

void PluginController::updatePluginList()
{
    QMap<QAction*, PLUGINPV>::iterator it(m_perVertexPlugins.begin());
    for(; it != m_perVertexPlugins.end(); ++it)
    {
        e_perVertexPluginsMenu->removeAction(it.key());
    }
    m_perVertexPlugins.clear();


    foreach (QString fileName, pluginsDir.entryList(QDir::Files))
    {
        QPluginLoader loader(pluginsDir.absoluteFilePath(fileName));
        QObject *plugin = loader.instance();
        if (plugin)
            populatePluginMenu(plugin,fileName);
    }
}

void PluginController::populatePluginMenu(QObject *plugin, const QString& fn )
{
    PerVertexInterface * iPerVertex = qobject_cast<PerVertexInterface *>(plugin);
    if(iPerVertex)
    {
        QStringList plugins = iPerVertex->perVertexPlugins();

        foreach (QString pluginName, plugins)
        {
            PLUGINPV p;
            p.pluginObject = iPerVertex;
            p.pluginName = pluginName;
            QAction * ac = e_perVertexPluginsMenu->addAction(pluginName);
            m_perVertexPlugins.insert(ac, p);
        }
    }
}

void PluginController::perVertexAction(QAction* act)
{
    SLHEMesh * mesh = dynamic_cast<SLHEMesh *>(e_object->object());
    if(!mesh)
        return;
    heds::HalfEdgeData *data = mesh->data();

    PLUGINPV p = m_perVertexPlugins[act];

    QList <QPair <QString , DataType> > attributeList;
    attributeList = p.pluginObject->attributes(p.pluginName);

    QDialog * diag = new QDialog();
    diag->setWindowTitle("Running ...");
    QProgressBar * pro = new QProgressBar(diag);
    pro->setMinimum(0);
    pro->setMaximum(attributeList.size()*data->numberOfVertices()-1);
    diag->show();

    for(int i = 0; i < attributeList.size(); ++i)
    {
        if(attributeList[i].first.startsWith("gl_"))
            continue;

        if(attributeList[i].first == "geometry" && attributeList[i].second != Vec3)
            continue;

        if(attributeList[i].first == "normal" && attributeList[i].second != Vec3)
            continue;

        if(attributeList[i].first == "texCoord" && attributeList[i].second != Vec2)
            continue;

        int attLoc = -1;
        if( attributeList[i].first != "geometry" &&
                attributeList[i].first != "normal" &&
                attributeList[i].first != "texCoord" &&
                attributeList[i].first != "null")
        {
            attLoc = mesh->findAttribute(attributeList[i].first);
            if(attLoc < 0)
                attLoc = mesh->addAttribute(PluginController::toProperty(attributeList[i]));
        }

        for(int j = 0; j < data->numberOfVertices(); ++j)
        {
            heds::Vertex * v = data->getVertex(j);
            QList<QVariant> value =  p.pluginObject->attributeValue(p.pluginName, i, v);

            pro->setValue(i*data->numberOfVertices() + j);

            if(attributeList[i].first == "null")
                continue;

            if(attributeList[i].first == "geometry")
            {
                QVector3D g = PluginController::toGeometry(value);
                v->setGeometry(QVector4D(g,1.0));
            }
            else if(attributeList[i].first == "normal")
            {
                QVector3D n = PluginController::toGeometry(value);
                v->setNormal(n);
            }else if(attributeList[i].first == "texCoord")
            {
                QVector2D uv = PluginController::toUV(value);
                v->setTexCoord(uv);
            }else
            {
                v->setAttribValue(attLoc, value);
            }
        }

        p.pluginObject->endOfVertices(p.pluginName, i);

    }

    diag->close();
    delete pro;
    delete diag;

    mesh->storeList();
}

PLYDataHeader::Property PluginController::toProperty(const QPair<QString, DataType>& pair)
{
    PLYDataHeader::Property p;

    p.name( pair.first );

    switch (pair.second) {
    case Vec2:
    case Vec3:
    case Vec4:
        p.setList(true);
        p.type( PLYDataHeader::Property::FLOAT );
        break;
    case IVec2:
    case IVec3:
    case IVec4:
        p.setList(true);
        p.type( PLYDataHeader::Property::INT );
        break;
    case Int:
        p.setList(false);
        p.type( PLYDataHeader::Property::INT );
        break;
    case Float:
        p.setList(false);
        p.type(PLYDataHeader::Property::FLOAT );
        break;
    }

    return p;
}

QVector3D PluginController::toGeometry(const QList<QVariant>& list)
{
    QVector3D geom;

    if(list.size() > 0)
    {
        geom.setX( list[0].value<float>() );
    }
    if(list.size() > 1)
    {
        geom.setY( list[1].value<float>() );
    }
    if(list.size() > 2)
    {
        geom.setZ( list[2].value<float>() );
    }
    return geom;
}

QVector2D PluginController::toUV(const QList<QVariant>& list)
{
    QVector2D uv;

    if(list.size() > 0)
    {
        uv.setX( list[0].value<float>() );
    }
    if(list.size() > 1)
    {
        uv.setY( list[1].value<float>() );
    }

    return uv;
}
