#include "GlobalProperties.h"
#include "ui_GlobalProperties.h"

GlobalProperties::GlobalProperties(MainWindow *parent) :
    QDockWidget(parent),
    ui(new Ui::GlobalProperties)
{
    ui->setupUi(this);

    QAction *act = toggleViewAction();
    act->setText("Global properties");
    act->setShortcut(QKeySequence::fromString("Ctrl+G"));
    parent->menuViewInsertAction(act);
}

GlobalProperties::~GlobalProperties()
{
    delete ui;
}

void GlobalProperties::setupProperties()
{
    QtVariantPropertyManager *variantManager = new QtVariantPropertyManager();
    QtProperty *topItem;
    QtVariantProperty *item;
    QtVariantEditorFactory *variantFactory;
    QtTreePropertyBrowser *variantEditor;

    topItem = variantManager->addProperty(QtVariantPropertyManager::groupTypeId(),
                QLatin1String("Group Property"));

    item = variantManager->addProperty(QVariant::Bool,
                                       QLatin1String("Bool Property"));
    item->setValue(true);
    topItem->addSubProperty(item);

    item = variantManager->addProperty(QVariant::Int,
                                       QLatin1String("Int Property"));
    item->setValue(20);
    item->setAttribute(QLatin1String("minimum"), 0);
    item->setAttribute(QLatin1String("maximum"), 100);
    item->setAttribute(QLatin1String("singleStep"), 10);
    topItem->addSubProperty(item);

    item = variantManager->addProperty(QVariant::Color,
                                       QLatin1String("Color Property"));
    topItem->addSubProperty(item);

    variantFactory = new QtVariantEditorFactory();

    variantEditor = new QtTreePropertyBrowser();
    variantEditor->setFactoryForManager(variantManager, variantFactory);
    variantEditor->addProperty(topItem);
    variantEditor->setPropertiesWithoutValueMarked(true);
    variantEditor->setRootIsDecorated(false);

    this->setWidget(variantEditor);
}
