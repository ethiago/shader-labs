#include "TexturePropertiesView.h"
#include "ui_TexturePropertiesView.h"
#include "Texture.h"
#include <QCloseEvent>

TexturePropertiesView::TexturePropertiesView(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::TexturePropertiesView)
{
    ui->setupUi(this);

    connect(ui->loadButton, SIGNAL(clicked()),
            this, SIGNAL(loadTextureClicked()));

    connect(ui->removeButton, SIGNAL(clicked()),
            this, SIGNAL(removeTextureClicked()));

    connect(ui->addButton, SIGNAL(clicked()),
            this, SIGNAL(addTextureClicked()));

    connect(ui->textureList, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(textureCurrentChange(int)));
}

TexturePropertiesView::~TexturePropertiesView()
{
    delete ui;
}

void TexturePropertiesView::setTexture(const Texture& texture)
{
    QImage img = texture.image();
    img = img.scaled(ui->imageContainer->pixmap()->size());

    ui->imageContainer->setPixmap(QPixmap::fromImage(img));
}

void TexturePropertiesView::closeEvent(QCloseEvent *event)
{
    emit s_closeEvent();
    event->accept();
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
