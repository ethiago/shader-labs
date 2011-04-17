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

    void fileControllerGetFileContent_data();
    void fileControllerGetFileContent();

    void fileControllerCompile1();
    void fileControllerCompile2();
    void fileControllerCompile3();
    void fileControllerCompile4();
    void fileControllerCompile5();
    void fileControllerCompile6();

    //void fileControllerSave_data();
    void fileControllerSave();

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
void FileControllerTeste::fileControllerGetFileContent_data()
{
    QTest::addColumn<QString>("filename1");
    QTest::addColumn<QString>("filename2");
    QTest::addColumn<bool>("result");

    QTest::newRow("equal files") << QString(":/OkVert")
                                  << QString(":/OkVert") << true;
    QTest::newRow("diferent files") << QString(":/OkVert")
                                  << QString(":/ProblemVert") << false;
    QTest::newRow("diferent files") << QString(":/NotExistFile")
                                  << QString(":/NotExistFile") << true;
}
void FileControllerTeste::fileControllerGetFileContent()
{
    QFETCH(QString, filename1);
    QFETCH(QString, filename2);
    QFETCH(bool, result);

    FileController fc(filename1, ShaderLab::Vertex);
    QFile f(filename2);
    if(f.open(QFile::ReadOnly))
    {
        QString fileContent = f.readAll();
        QVERIFY((fileContent == fc.getFileContent()) == result);
    }
    else
        QVERIFY(fc.getFileContent().isEmpty() == result);
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

/*void fileControllerSave_data()
{

}*/

void FileControllerTeste::fileControllerSave()
{
    QString before = "A seashell or sea shell, also known simply as a shell, is a hard, \
protective outer layer created by an animal that lives in the sea. The \
shell is part of the body of the animal.";

    QFile file(QString("../../ShaderLabUnitTestes/resources/testSave.vert"));
    file.open(QFile::WriteOnly | QIODevice::Text);
    QTextStream out(&file);
    out << before;
    file.close();

    QString content = "Test Content";
    FileController fc("../../ShaderLabUnitTestes/resources/testSave.vert",ShaderLab::Vertex);
    fc.save(content);

    if(file.open(QFile::ReadOnly))
        QVERIFY(file.readAll() == content);
    else
        qDebug() << "Erro";

}

void FileControllerTeste::mainWindowSetVisibleShader_data()
{
    using namespace ShaderLab;
    QTest::addColumn<int>("habilityShader");
    QTest::addColumn<bool>("visibility");
    QTest::addColumn<int>("visibilityShader");
    QTest::addColumn<bool>("result");
    QTest::addColumn<bool>("newVisibility");

    QTest::newRow("bla1") << shaderToInt(Vertex) <<  true
                          << shaderToInt(Vertex) << true << true;
    QTest::newRow("bla2") << shaderToInt(Vertex) <<  false
                          << shaderToInt(Vertex) << true << false;
    QTest::newRow("bla3") << shaderToInt(Vertex) << true
                          << shaderToInt(Fragment) << false << false;
    QTest::newRow("bla4") << shaderToInt(Vertex) << false
                          << shaderToInt(Fragment) << false << false;

}

void FileControllerTeste::mainWindowSetVisibleShader()
{
    using namespace ShaderLab;
    QFETCH(int, habilityShader);
    QFETCH(bool, visibility);
    QFETCH(int, visibilityShader);
    QFETCH(bool, result);
    QFETCH(bool, newVisibility);

    MainWindow w;
    w.addShader(intToShader(habilityShader));

    QVERIFY(w.setVisibleShader(visibility, intToShader(visibilityShader)) == result);
    QVERIFY(w.visibleShader(intToShader(visibilityShader)) == newVisibility);
}

void FileControllerTeste::mainWindowSetShaderCode() {}
void FileControllerTeste::mainWindowVisibleShader() {}
void FileControllerTeste::mainWindowShaderCode()    {}
void FileControllerTeste::mainWindowUpdateDisplay() {}
void FileControllerTeste::mainWindowOutputText()    {}

QTEST_MAIN(FileControllerTeste);

#include "tst_filecontrollerteste.moc"
