#include "TextureController.h"
#include "TexturePropertiesView.h"
#include "Texture.h"
#include "MainWindow.h"
#include <QImage>
#include <QFileDialog>
#include <QAction>

TextureController::TextureController(MainWindow* mw, QGLWidget* context,QObject *parent) :
    QObject(parent)
{
   m_textureList.append(Texture());

   m_context = context;
   m_textureView = new TexturePropertiesView(mw);
   mw->addDockWidget(Qt::LeftDockWidgetArea, m_textureView);

   {
   QAction *act = m_textureView->toggleViewAction();
   act->setText("Texture properties");
   act->setShortcut(QKeySequence::fromString("Ctrl+T"));
   mw->menuViewInsertAction(act);
   }

   connect(m_textureView, SIGNAL(loadTextureClicked()),
           this, SLOT(loadTexture()));

   connect(m_textureView, SIGNAL(removeTextureClicked()),
           this, SLOT(removeTexture()));

   connect(m_textureView, SIGNAL(addTextureClicked()),
           this, SLOT(addTexture()));

   connect(m_textureView, SIGNAL(textureCurrentChange(int)),
           this,SLOT(textureCurrentChange(int)));

   textureCurrentChange(0);
   viewUpdateList();

}

void TextureController::loadTexture(void)
{
    QString filename = QFileDialog::getOpenFileName(m_textureView, "Open Image", ".", tr("Images (*.png *.jpg *.tiff *.svg)"));

    if(!filename.isEmpty())
        textureFileName(filename);
}

void TextureController::addTexture(void)
{
    if(m_textureList[0].glTextureName() >= 0)
    {
        m_textureList.push_back(Texture());
        textureContext = m_textureList.size() - 1;
    }
    loadTexture();
}

void TextureController::textureFileName(const QString& imageFileName)
{
    QImage img(imageFileName);
    if(img.isNull())
    {
        qDebug() << "Nao eh uma imagem valida";
        return;
    }

    clearTexture();

    m_textureList[textureContext].setImage(img);
    m_textureList[textureContext].setGLTextureName(m_context->bindTexture(imageFileName, GL_TEXTURE_2D));
    m_textureList[textureContext].setVarName(SAMPLEPREFIX + QString::number(textureContext));
    activateTexture();
    viewUpdateList();
    m_context->updateGL();
}

void TextureController::clearTexture(void)
{
    if(m_textureList[textureContext].glTextureName() >= 0)
    {
        m_context->deleteTexture(m_textureList[textureContext].glTextureName());
        m_textureList[textureContext].clean();
    }
}
void TextureController::removeTexture(void)
{

    clearTexture();

    if(m_textureList.size() > 1)
    {
        m_textureList.removeAt(textureContext);
        if(textureContext == m_textureList.size())
            textureCurrentChange(textureContext-1);
        else
            textureCurrentChange(textureContext);
    }else
    {
        textureCurrentChange(textureContext);
    }

    viewUpdateList();
    activateTexture();
    m_context->updateGL();
}

void TextureController::textureCurrentChange(int index)
{
    if(index < 0)
        return;

    textureContext = index;
    m_textureView->setTexture(m_textureList[textureContext]);
    activateTexture();
    m_context->updateGL();
}

void TextureController::viewUpdateList(void)
{
    QList<QPair<QIcon, QString> > list;
    for(int i = 0; i < m_textureList.size(); ++i)
    {
        QPair<QIcon, QString> pair;
        pair.first = QIcon(QPixmap::fromImage(m_textureList[i].image()));
        pair.second = m_textureList[i].varName();
        list.push_back(pair);
    }

    m_textureView->setTextureList(list, textureContext);
}

void TextureController::applyTextures(QGLShaderProgram* program)
{
    for(int i = 0; i < m_textureList.size(); ++i)
    {
        const char * n = m_textureList[i].varName().toAscii();
        program->setUniformValue(n, (GLint)i );
    }
}

void TextureController::activateTexture(void)
{
    for(int i = 0; i < m_textureList.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        glBindTexture(GL_TEXTURE_2D, m_textureList[i].glTextureName());
    }
}
