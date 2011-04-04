#ifndef FILECONTROLLER_H
#define FILECONTROLLER_H

#include <QObject>
#include "mainwindow.h"
#include "global.h"

class FileController : public QObject
{
    Q_OBJECT
public:
    explicit FileController(MainWindow *v);
    QString getVertexCode(void);
    QString getFragmentCode(void);

public slots:
    void openFile(const QString& filename, ShaderLab::Shader);
    void closeShader(ShaderLab::Shader);

private:
    MainWindow * view;
    QString vertexFileName;
    QString fragmentFileName;

    bool vertexOpened;
    bool fragmentOpened;

};

#endif // FILECONTROLLER_H
