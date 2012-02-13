#ifndef EDITORCONTROLLER_H
#define EDITORCONTROLLER_H

#include <QObject>
#include <Global.h>

class SLFile;
class SLCodeContainer;

class EditorController : public QObject
{
    Q_OBJECT

    SLFile *file;
    SLCodeContainer *codeContainer;

public:
    explicit EditorController(const QString& filepath,
                              ShaderLab::Shader shadertype,
                              QObject *parent = 0);
    virtual ~EditorController();
private:
    bool closeShaderCode();
    bool saveFile();

signals:
    void useless();

public slots:
    void slot_closeShaderCode();
    void slot_saveFile();
    void fileChanged();
    void changeTabActivationStatus();

};

#endif // EDITORCONTROLLER_H
