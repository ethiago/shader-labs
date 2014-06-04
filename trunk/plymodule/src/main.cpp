#include <QCoreApplication>
#include <QStringList>
#include <QDebug>
#include "plyio.h"
#include <QFile>

QStringList models;
QString dir;

void testeNaoAbre()
{
    qDebug() << "testeNaoAbre";
    PLYIO plyio1;
    Q_ASSERT(plyio1.load(dir+models[0]) == false );
}

void testeMesmoArquivoIgual(int i)
{
    qDebug() << "testeMesmoArquivoIgual(" << i << ")";

        PLYIO plyio1;
        qDebug() << "lendo 1o";
        Q_ASSERT(plyio1.load(dir+models[i]));

        PLYIO plyio2;
        qDebug() << "lendo 2o";
        Q_ASSERT(plyio2.load(dir+models[i]));


        PLYData plydata1(plyio1.getData());
        PLYData plydata2(plyio2.getData());
        qDebug() << "comparando";
        Q_ASSERT( plydata1 == plydata2 );
}

void testeArquivosDiferentesDiferente(int i)
{
    qDebug() << "testeArquivosDiferentesDiferente(" << i << ")";
        PLYIO plyio1;
        Q_ASSERT(plyio1.load(dir+models[i]));

        PLYIO plyio2;
        Q_ASSERT(plyio2.load(dir+models[9]));


        PLYData plydata1(plyio1.getData());
        PLYData plydata2(plyio2.getData());
        Q_ASSERT( plydata1 != plydata2 );
}

void testeEscreveLeIgual(int i)
{
    qDebug() << "testeEscreveLeIgual(" << i << ")";
    PLYIO plyio1;
    Q_ASSERT(plyio1.load(dir+models[i]));

    Q_ASSERT(plyio1.write("model.ply"));

    PLYIO plyio2;
    Q_ASSERT(plyio2.load("model.ply"));

    PLYData plydata1(plyio1.getData());
    PLYData plydata2(plyio2.getData());
    Q_ASSERT( plydata1 == plydata2 );
}

int main(int argc, char *argv[])
{
    QCoreApplication a(argc, argv);
    dir = QString("../plymodule/models/");
    models.append("armadillo.ply");
    models.append("bun_zipper.ply");
    models.append("bun_zipper_res2.ply");
    models.append("bun_zipper_res3.ply");
    models.append("bun_zipper_res4.ply");
    models.append("dragon_vrip.ply");
    models.append("drill_shaft_vrip.ply");
    models.append("drill_shaft_zip.ply");
    models.append("happy_vrip.ply");
    models.append("cubo.ply");

    testeNaoAbre();
    for(int i = 1; i < models.size(); ++i)
        testeMesmoArquivoIgual(i);

    for(int i = 1; i < models.size()-1; ++i)
        testeArquivosDiferentesDiferente(i);

    for(int i = 1; i < models.size(); ++i)
        testeEscreveLeIgual(i);

    Q_ASSERT(false); return a.exec();
}
