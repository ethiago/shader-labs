#ifndef TEXTUREPROPERTIESVIEW_H
#define TEXTUREPROPERTIESVIEW_H

#include <QDockWidget>

namespace Ui {
    class TexturePropertiesView;
}

class Texture;

class TexturePropertiesView : public QDockWidget
{
    Q_OBJECT

signals:
    void loadTextureClicked(void);
    void removeTextureClicked(void);

public:
    explicit TexturePropertiesView(QWidget *parent = 0);
    ~TexturePropertiesView();

    void setTexture(const Texture& texture);

private:
    Ui::TexturePropertiesView *ui;
};

#endif // TEXTUREPROPERTIESVIEW_H
