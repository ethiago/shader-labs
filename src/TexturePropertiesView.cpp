#include "TexturePropertiesView.h"
#include "ui_TexturePropertiesView.h"
#include "Texture.h"

TexturePropertiesView::TexturePropertiesView(QWidget *parent) :
    QDockWidget(parent),
    ui(new Ui::TexturePropertiesView)
{
    ui->setupUi(this);

    connect(ui->loadButton, SIGNAL(clicked()),
            this, SIGNAL(loadTextureClicked()));

    connect(ui->removeButton, SIGNAL(clicked()),
            this, SIGNAL(removeTextureClicked()));
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
