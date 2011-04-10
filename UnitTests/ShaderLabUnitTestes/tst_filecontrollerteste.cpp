#include <QtCore/QString>
#include <QtTest/QtTest>
#include <QFile>

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

QTEST_APPLESS_MAIN(FileControllerTeste);

#include "tst_filecontrollerteste.moc"
