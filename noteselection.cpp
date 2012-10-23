#include "noteselection.h"
#include "ui_noteselection.h"

noteSelection::noteSelection(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::noteSelection)
{
    ui->setupUi(this);

    connect(ui->SelectAll,SIGNAL(clicked()),this,SLOT(CheckAll()));
    connect(ui->SelectAllDifferent,SIGNAL(clicked()),this,SLOT(CheckAllDifferent()));
    connect(ui->DeselectAll,SIGNAL(clicked()),this,SLOT(DecheckAll()));
}

noteSelection::~noteSelection()
{
    delete ui;
}

void noteSelection::AddItem(QString txt,bool Checkable,bool Checked)
{
    QListWidgetItem* item = new QListWidgetItem(txt,ui->listWidget);

    if (Checkable)
    {
        item->setFlags(item->flags() | Qt::ItemIsUserCheckable);
    }
    if (Checked)
    {
        item->setCheckState(Qt::Checked);
    }
    else
    {
        item->setCheckState(Qt::Unchecked);
    }
}

void noteSelection::SetMatchList(bool* matchList)
{
    this->m_mtchList = matchList;
}

void noteSelection::CheckAll(void)
{
    int i;
    for (i = 0; i < ui->listWidget->count(); i++)
    {
        QListWidgetItem* item = ui->listWidget->item(i);
        item->setCheckState(Qt::Checked);
    }
}

void noteSelection::DecheckAll(void)
{
    int i;
    for (i = 0; i < ui->listWidget->count(); i++)
    {
        QListWidgetItem* item = ui->listWidget->item(i);
        item->setCheckState(Qt::Unchecked);
    }
}

void noteSelection::CheckAllDifferent(void)
{
    int i;
    for (i = 0; i < ui->listWidget->count(); i++)
    {
        QListWidgetItem* item = ui->listWidget->item(i);
        if (m_mtchList[i])
        {
            item->setCheckState(Qt::Unchecked);
        }
        else
        {
            item->setCheckState(Qt::Checked);
        }
    }
}

void noteSelection::GetSelectedList(bool* selectedList)
{
    int i;
    for (i = 0; i < ui->listWidget->count(); i++)
    {
        QListWidgetItem* item = ui->listWidget->item(i);
        if (item->checkState() == Qt::Checked)
        {
            selectedList[i] = true;
        }
        else
        {
            selectedList[i] = false;
        }
    }
}
