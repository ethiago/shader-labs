#include "Find.h"
#include "ui_Find.h"

Find::Find(QWidget *parent) :
    QWidget(parent),
    ui(new Ui::Find)
{
   ui->setupUi(this);

   m_toggleViewAction = new QAction(this);
   m_toggleViewAction->setCheckable(true);

   okVisible = true;

   connect(m_toggleViewAction, SIGNAL(triggered(bool)),
           this, SLOT(toggle(bool)));

   connect(ui->nextButton, SIGNAL(clicked()),
           this, SLOT(findNext()));
   connect(ui->backButton, SIGNAL(clicked()),
           this, SLOT(findBack()));
   connect(ui->closeButton, SIGNAL(clicked()),
           this, SLOT(closeButton()));
   /*connect(ui->replaceButton, SIGNAL(clicked()),
           this, SLOT(replaceNext()));
   connect(ui->replaceAllButton, SIGNAL(clicked()),
           this, SLOT(replaceAll()));
           */

}

QAction* Find::toggleViewAction(void)
{
    return m_toggleViewAction;
}

void Find::setVisibility(bool b)
{
    m_toggleViewAction->setChecked(b);
    setVisible(b);
}

void Find::toggle(bool b)
{
    if(b)
    {
        if(okVisible)
            setVisible(b);
        else
            m_toggleViewAction->setChecked(false);
    }else
        setVisible(b);
}

void Find::setOkVisible(bool b)
{
    okVisible = b;
    if(!b)
        setVisibility(false);
}

void Find::findNext()
{
    if(!ui->findText->text().isEmpty())
        emit findNext(ui->findText->text());
}

void Find::findBack()
{
    if(!ui->findText->text().isEmpty())
        emit findBack(ui->findText->text());
}

void Find::replaceNext()
{
    //if(!ui->findText->text().isEmpty() && !ui->replaceText->text().isEmpty())
    //    emit replaceNext(ui->findText->text(), ui->replaceText->text());
}

void Find::replaceAll()
{
    //if(!ui->findText->text().isEmpty() && !ui->replaceText->text().isEmpty())
    //    emit replaceAll(ui->findText->text(), ui->replaceText->text());
}

void Find::closeButton()
{
    m_toggleViewAction->setChecked(false);
    toggle(false);
}
