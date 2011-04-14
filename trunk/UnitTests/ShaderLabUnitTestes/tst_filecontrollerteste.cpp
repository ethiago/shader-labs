#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QFile>
#include <QDebug>
#include <QGLWidget>

#include "../../headers/filecontroller.h"
#include "../../headers/mainwindow.h"
#include "../../headers/global.h"

class FileControllerTeste : public QObject
{
    Q_OBJECT

public:
    FileControllerTeste();

private Q_SLOTS:

    void fileControllerGetFileContent1();
    void fileControllerGetFileContent2();
    void fileControllerGetFileContent3();

    void fileControllerCompile1();
    void fileControllerCompile2();
    void fileControllerCompile3();
    void fileControllerCompile4();
    void fileControllerCompile5();
    void fileControllerCompile6();

    void mainWindowSetVisibleShader_data();
    void mainWindowSetVisibleShader();

    void mainWindowSetShaderCode();
    void mainWindowVisibleShader();
    void mainWindowShaderCode();
    void mainWindowUpdateDisplay();
    void mainWindowOutputText();

};

FileControllerTeste::FileControllerTeste()
{

}
void FileControllerTeste::fileControllerGetFileContent1()
{
    FileController fc(":/OkVert", ShaderLab::Vertex);
    QFile f(":/OkVert");
    f.open(QFile::ReadOnly);
    QString fileContent = f.readAll();
    QVERIFY(fileContent == fc.getFileContent());
}
void FileControllerTeste::fileControllerGetFileContent2()
{
    FileController fc(":/OkVert", ShaderLab::Vertex);
    QFile f(":/ProblemVert");
    f.open(QFile::ReadOnly);
    QString fileContent = f.readAll();
    QVERIFY(fileContent != fc.getFileContent());
}
void FileControllerTeste::fileControllerGetFileContent3()
{
    FileController fc(":/NotExistFile", ShaderLab::Vertex);
    QVERIFY(QString() == fc.getFileContent());
}
void FileControllerTeste::fileControllerCompile1()
{
    FileController fc(":/OkVert", ShaderLab::Vertex);
    QFile f(":/OkVert");
    f.open(QFile::ReadOnly);
    QString fileContent = f.readAll();
    QGLShader shader(QGLShader::Vertex);

    QVERIFY(fc.compile(fileContent) == shader.compileSourceCode(fileContent));
}

void FileControllerTeste::fileControllerCompile2()
{
    FileController fc(":/OkVert", ShaderLab::Vertex);
    QGLShader shader(QGLShader::Vertex);
    QVERIFY(fc.compile() == shader.compileSourceFile(":/OkVert"));
}

void FileControllerTeste::fileControllerCompile3()
{
    FileController fc(":/ProblemVert", ShaderLab::Vertex);
    QFile f(":/ProblemVert");
    f.open(QFile::ReadOnly);
    QString fileContent = f.readAll();
    QGLShader shader(QGLShader::Vertex);

    QVERIFY(fc.compile(fileContent) == shader.compileSourceCode(fileContent));
}

void FileControllerTeste::fileControllerCompile4()
{
    FileController fc(":/ProblemVert", ShaderLab::Vertex);
    QGLShader shader(QGLShader::Vertex);
    QVERIFY(fc.compile() == shader.compileSourceFile(":/ProblemVert"));
}

void FileControllerTeste::fileControllerCompile5()
{
    FileController fc(":/NotExistFile", ShaderLab::Vertex);
    QGLShader shader(QGLShader::Vertex);
    QVERIFY(fc.compile() == shader.compileSourceFile(":/NotExistFile"));
}

void FileControllerTeste::fileControllerCompile6()
{
    FileController fc(":/NotExistFile", ShaderLab::Vertex);
    QVERIFY(!fc.compile());
}

void FileControllerTeste::mainWindowSetVisibleShader_data()
{
    QTest::addColumn<bool>("visibility");
    QTest::addColumn<bool>("result");

    QTest::newRow("bla1") << true << true;
    QTest::newRow("bla2") << false << true;
}

void FileControllerTeste::mainWindowSetVisibleShader()
{
    QFETCH(bool, visibility);
    QFETCH(bool, result);

    MainWindow w;
    w.addShader(ShaderLab::Vertex);

    QVERIFY(w.setVisibleShader(visibility, ShaderLab::Vertex) == result);
    QVERIFY(w.visibleShader(ShaderLab::Vertex) == visibility);
}

void FileControllerTeste::mainWindowSetShaderCode() {}
void FileControllerTeste::mainWindowVisibleShader() {}
void FileControllerTeste::mainWindowShaderCode()    {}
void FileControllerTeste::mainWindowUpdateDisplay() {}
void FileControllerTeste::mainWindowOutputText()    {}

QTEST_MAIN(FileControllerTeste);

#include "tst_filecontrollerteste.moc"
