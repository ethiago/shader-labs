#ifndef TEXTUREPROPERTIESVIEW_H
#define TEXTUREPROPERTIESVIEW_H

#include <QDockWidget>
#include <QList>


namespace Ui {
    class TexturePropertiesView;
}

class Texture;
class MainWindow;

class TexturePropertiesView : public QDockWidget
{
    Q_OBJECT

signals:
    void loadTextureClicked(void);
    void removeTextureClicked(void);
    void addTextureClicked(void);
    void textureCurrentChange(int);

public:
    explicit TexturePropertiesView(MainWindow *mw);
    ~TexturePropertiesView();

    void setTexture(const Texture& texture);

    void setTextureList(const QList<QPair<QIcon, QString> >&, int currInd = 0);

    void removeItem(int idx);

    void addItem(const QIcon&, const QString&);

    void clear();

    int current()const;

    void setCurrent(int idx);

private:
    Ui::TexturePropertiesView *ui;
};

#endif // TEXTUREPROPERTIESVIEW_H
