#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QFile>
#include <QDebug>

#include "../../headers/filecontroller.h"
#include "../../headers/global.h"

class FileControllerTeste : public QObject
{
    Q_OBJECT

public:
    FileControllerTeste();

private Q_SLOTS:
    void getFileContent1();
    void getFileContent2();
    void getFileContent3();

    void compile1();
    void compile2();
    void compile3();
    void compile4();
    void compile5();
    void compile6();

};

FileControllerTeste::FileControllerTeste()
{

}

void FileControllerTeste::getFileContent1()
{
    FileController fc(":/OkVert", ShaderLab::Vertex);
    QFile f(":/OkVert");
    f.open(QFile::ReadOnly);
    QString fileContent = f.readAll();
    QVERIFY(fileContent == fc.getFileContent());
}

void FileControllerTeste::getFileContent2()
{
    FileController fc(":/OkVert", ShaderLab::Vertex);
    QFile f(":/ProblemVert");
    f.open(QFile::ReadOnly);
    QString fileContent = f.readAll();
    QVERIFY(fileContent != fc.getFileContent());
}

void FileControllerTeste::getFileContent3()
{
    FileController fc(":/NotExistFile", ShaderLab::Vertex);
    QVERIFY(QString() == fc.getFileContent());
}

void FileControllerTeste::compile1()
{
    FileController fc(":/OkVert", ShaderLab::Vertex);
    QFile f(":/OkVert");
    f.open(QFile::ReadOnly);
    QString fileContent = f.readAll();
    QVERIFY(fc.compile(fileContent));
}

void FileControllerTeste::compile2()
{
    FileController fc(":/OkVert", ShaderLab::Vertex);
    QVERIFY(fc.compile());
}

void FileControllerTeste::compile3()
{
    FileController fc(":/ProblemVert", ShaderLab::Vertex);
    QFile f(":/ProblemVert");
    f.open(QFile::ReadOnly);
    QString fileContent = f.readAll();
    QVERIFY(!fc.compile(fileContent));
}

void FileControllerTeste::compile4()
{
    FileController fc(":/ProblemVert", ShaderLab::Vertex);
    QVERIFY(!fc.compile());
}

void FileControllerTeste::compile5()
{
    FileController fc(":/NotExistFile", ShaderLab::Vertex);
    QVERIFY(!fc.compile(QString()));
}

void FileControllerTeste::compile6()
{
    FileController fc(":/NotExistFile", ShaderLab::Vertex);
    QVERIFY(!fc.compile());
}

QTEST_APPLESS_MAIN(FileControllerTeste);

#include "tst_filecontrollerteste.moc"
