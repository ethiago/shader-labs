#include "TexturePropertiesView.h"
#include "ui_TexturePropertiesView.h"
#include "Texture.h"
#include "MainWindow.h"
#include <QCloseEvent>

TexturePropertiesView::TexturePropertiesView(MainWindow *parent) :
    QDockWidget(parent),
    ui(new Ui::TexturePropertiesView)
{
    ui->setupUi(this);

    ui->add3DButton->setVisible(false);

    QAction *act = toggleViewAction();
    act->setText("Texture properties");
    act->setShortcut(QKeySequence::fromString("Ctrl+T"));

    parent->menuViewInsertAction(act);
    parent->addDockWidget(Qt::LeftDockWidgetArea, this);

    connect(ui->loadButton, SIGNAL(clicked()),
            this, SIGNAL(loadTextureClicked()));

    connect(ui->removeButton, SIGNAL(clicked()),
            this, SIGNAL(removeTextureClicked()));

    connect(ui->addButton, SIGNAL(clicked()),
            this, SIGNAL(addTextureClicked()));

    connect(ui->textureList, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(textureCurrentChange(int)));

//    connect(ui->add3DButton, SIGNAL(clicked()),
//            this, SIGNAL(addTexture3DClicked()));

    setVisible(false);
}

TexturePropertiesView::~TexturePropertiesView()
{
    delete ui;
}

void TexturePropertiesView::setTexture(const Texture& texture)
{
    QImage img = texture.image();
    img = img.scaled(ui->imageContainer->size());

    ui->imageContainer->setPixmap(QPixmap::fromImage(img));

    ui->textureName->setText(texture.fileName());
    ui->textureName->setToolTip(texture.fullFileName());
}

void TexturePropertiesView::setTextureList(const QList<QPair<QIcon, QString> >& textureList,
                                           int currInd)
{
    ui->textureList->clear();
    for(int i = 0; i < textureList.size(); ++i)
    {
        ui->textureList->addItem(textureList[i].first,textureList[i].second);
    }

    ui->textureList->setCurrentIndex(currInd);
}
