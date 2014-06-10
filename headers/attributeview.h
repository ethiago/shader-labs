#ifndef ATTRIBUTEVIEW_H
#define ATTRIBUTEVIEW_H

#include <QWidget>
#include <QVariant>
#include <QStringList>
#include <QFormLayout>

namespace Ui {
class AttributeView;
}

class QComboBox;
class QLineEdit;

class AttributeView : public QWidget
{
    Q_OBJECT

    QList<QComboBox*> attTypes;
    QList<QLineEdit*> attNames;

    QList<QComboBox*> faceTypes;
    QList<QLineEdit*> faceNames;

    QList<QComboBox*> uniformsTypes;
    QList<QLineEdit*> uniformsNames;

    QStringList typesView;

public:
    explicit AttributeView(QWidget *parent = 0);
    ~AttributeView();

    void addAttribute(QVariant::Type type, const QString& name, bool isList = false);

    void addFaceUniform(QVariant::Type type, const QString& name, bool isList = false);

    void addUniform(QVariant::Type type, const QString& name, bool isList = false);

    void clear();

private:
    Ui::AttributeView *ui;

    void addItem(QVariant::Type type, const QString& name, bool isList, QFormLayout* layout, QList<QComboBox*>*,QList<QLineEdit*>* );
};

#endif // ATTRIBUTEVIEW_H
