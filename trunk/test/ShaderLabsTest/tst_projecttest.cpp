#include <QtCore/QString>
#include <QtTest/QtTest>

class ProjectTest : public QObject
{
    Q_OBJECT

public:
    ProjectTest();

private Q_SLOTS:
    void testCase1();
    void testCase1_data();
};

ProjectTest::ProjectTest()
{
}

void ProjectTest::testCase1()
{
    QFETCH(QString, in);
    QFETCH(QString, out);
    QString msg = QString("Test Caso 1 com entrada ") + in + QString("e saida ") + out;

    QVERIFY2(in == out, msg.toAscii());
}

void ProjectTest::testCase1_data()
{
    QTest::addColumn<QString>("in");
    QTest::addColumn<QString>("out");
    QTest::newRow("0") << QString() << QString("");
}

QTEST_APPLESS_MAIN(ProjectTest);

#include "tst_projecttest.moc"
