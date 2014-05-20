#include "TexturePropertiesView.h"
#include "ui_TexturePropertiesView.h"
#include "Texture.h"
#include "MainWindow.h"
#include <QCloseEvent>

TexturePropertiesView::TexturePropertiesView(MainWindow *mw) :
    QDockWidget(NULL),
    ui(new Ui::TexturePropertiesView)
{
    ui->setupUi(this);

    QAction *act = toggleViewAction();
    act->setText("Texture properties");
    act->setShortcut(QKeySequence::fromString("Ctrl+T"));

    mw->menuViewInsertAction(act);
    mw->addDockWidget(Qt::RightDockWidgetArea, this);

    connect(ui->loadButton, SIGNAL(clicked()),
            this, SIGNAL(loadTextureClicked()));

    connect(ui->removeButton, SIGNAL(clicked()),
            this, SIGNAL(removeTextureClicked()));

    connect(ui->addButton, SIGNAL(clicked()),
            this, SIGNAL(addTextureClicked()));

    connect(ui->textureList, SIGNAL(currentIndexChanged(int)),
            this, SIGNAL(textureCurrentChange(int)));

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
    ui->textureName->setToolTip(texture.filePath());
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

void TexturePropertiesView::removeItem(int idx)
{
    ui->textureList->removeItem(idx);
}

void TexturePropertiesView::addItem(const QIcon& ico, const QString& name)
{
    ui->textureList->addItem(ico,name);
    ui->textureList->setCurrentIndex(ui->textureList->count()-1);
}

void TexturePropertiesView::clear()
{
    ui->textureList->clear();
    ui->imageContainer->setPixmap(QPixmap::fromImage(QImage(":/ico/noImage")));
    ui->textureName->setText("");
    ui->textureName->setToolTip("");

}

int TexturePropertiesView::current()const
{
    return ui->textureList->currentIndex();
}

void TexturePropertiesView::setCurrent(int idx)
{
    if(idx < ui->textureList->count())
        ui->textureList->setCurrentIndex(idx);
}
