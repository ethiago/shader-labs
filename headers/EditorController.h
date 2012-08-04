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
    SLCodeContainer *m_codeContainer;
    ShaderLab::Shader m_type;

public:
    explicit EditorController(ShaderLab::Shader shadertype,
                              const QString& filepath = QString(),
                              QObject *parent = 0);
    virtual ~EditorController();

    SLCodeContainer * codeContainer(void);
    bool closeShaderCode();
    bool saveFile(bool forceNew = false);
    bool isActive();
    bool isNew();
    QString getContent();
    QString getContentFile();
    const SLFile& getFile();

signals:
    void useless(ShaderLab::Shader);

public slots:
    void slot_closeShaderCode();
    void slot_saveFile();
    void saveAs();
    void fileChanged();
    void changeTabActivationStatus();



};

#endif // EDITORCONTROLLER_H
