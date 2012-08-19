#include "SLTextures.h"
#include "Texture3D.h"
#include "TexturePropertiesView.h"
#include "Texture.h"
#include "MainWindow.h"
#include <QImage>
#include <QFileDialog>
#include <QAction>
#include "InterfaceRequests.h"
#include "SLTexture3dDialog.h"
#include "SLShaderProgram.h"
#include <QGLWidget>

SLTextures::SLTextures(TexturePropertiesView* tpv, SLTexture3DDialog * t3d, QObject* parent) :
    QObject(parent), m_textureView(tpv), m_texture3DDialog(t3d)
{
   m_textureList.append(Texture());

   connect(m_textureView, SIGNAL(loadTextureClicked()),
           this, SLOT(changeTexture()));

   connect(m_textureView, SIGNAL(removeTextureClicked()),
           this, SLOT(removeTexture()));

   connect(m_textureView, SIGNAL(addTextureClicked()),
           this, SLOT(addTexture()));

   connect(m_textureView, SIGNAL(textureCurrentChange(int)),
           this,SLOT(textureCurrentChange(int)));

   connect(m_textureView, SIGNAL(addTexture3DClicked()),
           this, SLOT(addTexture3D()));

   textureCurrentChange(0);
   viewUpdateList();
}

SLTextures::~SLTextures()
{
    disconnect(this);
}

void SLTextures::changeTexture(void)
{
    QString filename = QFileDialog::getOpenFileName(m_textureView, "Open Image", ".", tr("Images (*.png *.jpg *.tiff *.svg)"));

    setupTexture(filename, false);
}

void SLTextures::addTexture(void)
{
    QString filename = QFileDialog::getOpenFileName(m_textureView, "Open Image", ".", tr("Images (*.png *.jpg *.tiff *.svg)"));

    setupTexture(filename, true);
}

void SLTextures::addTexture3D(void)
{
    QString filename = QFileDialog::getOpenFileName(m_textureView, "Open Raw File", ".", "*.raw");

    if(filename.isEmpty())
        return;

    QFile f(filename);
    int size = f.size();

    m_texture3DDialog->setFileName(filename);
    int ret = m_texture3DDialog->exec();
    if(ret != QDialog::Accepted)
        return;

    int w = m_texture3DDialog->getFileWidth();
    int h = m_texture3DDialog->getFileHeight();
    int d = m_texture3DDialog->getFileDepth();
    int l = m_texture3DDialog->getLengthData();
    int informedSize = w * h * d * l;
    if( informedSize != size)
    {
        InterfaceRequests::sizeFileNotMatch(size, informedSize);
        return;
    }

    if(w*h*d > 5832000)
    {
        InterfaceRequests::fileTooLarge(w*h*d);
        return;
    }

    Texture3D tex;
    tex.loadFromRawFile(filename, w, h, d,l);
    tex.openGLBind();
    Texture tx(tex);
    if(m_textureList[0].glTextureName() >= 0)
    {
            m_textureList.push_back(tx);
            textureContext = m_textureList.size() - 1;
    }else
        m_textureList[textureContext] = tx;

    m_textureList[textureContext].setVarName(SAMPLEPREFIX3D + QString::number(textureContext));
    activeTexture();
    viewUpdateList();
    ShaderLab::instance()->glContext()->updateGL();
}

void SLTextures::setupTexture(const QString& imageFileName, bool add)
{
    ShaderLab *sl = ShaderLab::instance();

    if(imageFileName.isEmpty())
        return;

    if(!Texture::isValid(imageFileName))
    {
        InterfaceRequests::openFileProblem(imageFileName);
        return;
    }

    if(add && m_textureList[0].glTextureName() >= 0)
    {
            m_textureList.push_back(Texture());
            textureContext = m_textureList.size() - 1;
    }

    QImage img(imageFileName);

    clearTexture();

    m_textureList[textureContext].setImage(img);
    m_textureList[textureContext].setGLTextureName(sl->glContext()->bindTexture(imageFileName, GL_TEXTURE_2D));
    m_textureList[textureContext].setVarName(SAMPLEPREFIX2D + QString::number(textureContext));
    m_textureList[textureContext].setFileName(imageFileName);
    activeTexture();
    viewUpdateList();
    sl->glContext()->updateGL();
}

void SLTextures::clearTexture(void)
{
    ShaderLab *sl = ShaderLab::instance();
    if(m_textureList[textureContext].glTextureName() >= 0)
    {

        sl->glContext()->deleteTexture(m_textureList[textureContext].glTextureName());
        m_textureList[textureContext].clean();
    }
}

void SLTextures::remakeVarNames()
{
    for(int i = 0; i < m_textureList.size(); i++)
    {
        if(m_textureList[i].is2D())
            m_textureList[i].setVarName(SAMPLEPREFIX2D + QString::number(i));
        else
            m_textureList[i].setVarName(SAMPLEPREFIX3D + QString::number(i));
    }
}

void SLTextures::removeTexture(void)
{

    ShaderLab *sl = ShaderLab::instance();
    clearTexture();

    if(m_textureList.size() > 1)
    {
        m_textureList.removeAt(textureContext);
        remakeVarNames();
        if(textureContext == m_textureList.size())
            textureCurrentChange(textureContext-1);
        else
            textureCurrentChange(textureContext);
    }else
    {
        textureCurrentChange(textureContext);
    }

    viewUpdateList();
    activeTexture();
    sl->glContext()->updateGL();
}

void SLTextures::textureCurrentChange(int index)
{
    ShaderLab *sl = ShaderLab::instance();
    if(index < 0)
        return;

    textureContext = index;
    m_textureView->setTexture(m_textureList[textureContext]);
    activeTexture();
    sl->glContext()->updateGL();
}

void SLTextures::viewUpdateList(void)
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

void SLTextures::setUniformTextureNames(SLShaderProgram* program)
{
    for(int i = 0; i < m_textureList.size(); ++i)
    {
        const char * n = m_textureList[i].varName().toAscii();
        program->setUniformValue(n, (GLint)i );
    }
}

void SLTextures::activeTexture(void)
{
    for(int i = 0; i < m_textureList.size(); ++i)
    {
        glActiveTexture(GL_TEXTURE0 + i);
        if(m_textureList[i].is2D())
            glBindTexture(GL_TEXTURE_2D, m_textureList[i].glTextureName());
        else
            glBindTexture(GL_TEXTURE_3D, m_textureList[i].glTextureName());
    }
}

QStringList SLTextures::getTextureFileNames()
{
    QStringList filenames;

    for(int i = 0; i < m_textureList.size(); ++i)
    {
        QString filename = m_textureList[i].fullFileName();
        if(!filename.isEmpty())
        {
            filenames.append(filename);
        }
    }

    return filenames;
}

void SLTextures::removeAllTextures(void)
{
    textureContext = m_textureList.size() - 1;

    while(textureContext > 0)
        removeTexture();

    clearTexture();
}

void SLTextures::setTextures(const QStringList& list)
{
    removeAllTextures();

    for(int i = 0; i < list.size(); ++i)
    {
        setupTexture(list[i], true);
    }

    viewUpdateList();
    activeTexture();
    ShaderLab::instance()->glContext()->updateGL();
}
