#ifndef RENDERCONTROLLER_H
#define RENDERCONTROLLER_H

#include <QObject>

//class GLDisplay;
class MainWindow;

class RenderController : public QObject
{
    Q_OBJECT

    //GLDisplay *display;

public:
    explicit RenderController(const MainWindow* mainWindow,
                              QObject *parent = 0);

signals:
    void drawModel(void);

public slots:

};

#endif // RENDERCONTROLLER_H
