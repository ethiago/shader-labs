#ifndef TEXTUREPROPERTIESVIEW_H
#define TEXTUREPROPERTIESVIEW_H

#include <QDockWidget>
#include <QList>

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
    void addTextureClicked(void);
    void s_closeEvent();
    void textureCurrentChange(int);

public:
    explicit TexturePropertiesView(QWidget *parent = 0);
    ~TexturePropertiesView();

    void setTexture(const Texture& texture);

    void setTextureList(const QList<QPair<QIcon, QString> >&, int);

private:
    Ui::TexturePropertiesView *ui;

    void closeEvent(QCloseEvent *event);
};

#endif // TEXTUREPROPERTIESVIEW_H
