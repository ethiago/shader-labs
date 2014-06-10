#include "attributeview.h"
#include "ui_attributeview.h"
#include <QComboBox>
#include <QLineEdit>

AttributeView::AttributeView(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::AttributeView)
{
    ui->setupUi(this);

    typesView.append("int");
    typesView.append("float");
    typesView.append("ivec");
    typesView.append("vec");
}

AttributeView::~AttributeView()
{
    delete ui;
}

void AttributeView::addAttribute(QVariant::Type type, const QString& name, bool isList)
{
    addItem(type, name, isList, ui->attributeLayout, &attTypes, &attNames);
}

void AttributeView::addFaceUniform(QVariant::Type type, const QString& name, bool isList)
{
    addItem(type, name, isList, ui->uniformFaceLayout, &faceTypes, &faceNames);
}

void AttributeView::addUniform(QVariant::Type type, const QString& name, bool isList)
{
    addItem(type, name, isList, ui->uniformLayout, &uniformsTypes, &uniformsNames);
}

void AttributeView::addItem(QVariant::Type type, const QString& name, bool isList, QFormLayout* layout, QList<QComboBox *> * c, QList<QLineEdit *> * l)
{
    Q_ASSERT(type == QVariant::Int || type == QVariant::Double);

    QLineEdit *line = new QLineEdit();
    line->setText(name);
    line->setEnabled(false);

    QComboBox * combo = new QComboBox();
    combo->addItems(typesView);

    int current = 0;
    if(type == QVariant::Double)
        ++current;
    if(isList)
        current+=2;

    combo->setCurrentIndex(current);
    combo->setEnabled(false);

    c->append(combo);
    l->append(line);

    layout->addRow(combo, line);
}

void AttributeView::clear()
{
    //Attributes
    for(int i = 0; i < attNames.size(); ++i)
    {
        ui->attributeLayout->removeWidget(attTypes[i]);
        ui->attributeLayout->removeWidget(attNames[i]);

        delete attTypes[i];
        delete attNames[i];
    }

    attTypes.clear();
    attNames.clear();

    //Face Uniforms
    for(int i = 0; i < faceNames.size(); ++i)
    {
        ui->uniformFaceLayout->removeWidget(faceTypes[i]);
        ui->uniformFaceLayout->removeWidget(faceNames[i]);

        delete faceTypes[i];
        delete faceNames[i];
    }

    faceTypes.clear();
    faceNames.clear();

    //Uniforms
    for(int i = 0; i < uniformsNames.size(); ++i)
    {
        ui->uniformLayout->removeWidget(uniformsTypes[i]);
        ui->uniformLayout->removeWidget(uniformsNames[i]);

        delete uniformsTypes[i];
        delete uniformsNames[i];
    }

    uniformsTypes.clear();
    uniformsNames.clear();
}
