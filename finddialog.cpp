#include "finddialog.h"
#include "ui_finddialog.h"

FindDialog::FindDialog(GAPP_Data* data, QString strToBeFind, QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindDialog),
    m_data(data),
    m_strToBeFind(strToBeFind)
{
    ui->setupUi(this);
    ui->stringToBeFound->setText(strToBeFind);

    connect (ui->stringToBeFound, SIGNAL(keyPressed(int)),
             this, SLOT(on_stringToBeFound_receiveKeyPress(int)));
    connect (ui->searchResults, SIGNAL(keyPressed(int)),
             this, SLOT(on_searchResults_receiveKeyPress(int)));
}

FindDialog::~FindDialog()
{
    delete ui;
}

void FindDialog::on_stringToBeFound_textChanged(const QString &arg1)
{
    m_strToBeFind = arg1;
    m_data->FindInNotes(&m_resultStrList, &m_noteSelectedList, & m_numFindRequiredList,arg1);
    ui->searchResults->clear();
    foreach (QString str , m_resultStrList)
    {
        ui->searchResults->addItem(str);
    }
}

void FindDialog::on_searchResults_currentRowChanged(int currentRow)
{
    if ((currentRow >= 0) && (currentRow < m_noteSelectedList.size()))
    {
        m_noteSelected = m_noteSelectedList.at(currentRow);
        m_numFindRequired = m_numFindRequiredList.at(currentRow);
    }
}

void FindDialog::on_stringToBeFound_receiveKeyPress(int key)
{
    if (key == Qt::Key_Down)
    {
        if (ui->searchResults->count() != 0)
        {
            ui->searchResults->setFocus();
            ui->searchResults->setCurrentRow(0);
        }
    }
}

void FindDialog::on_searchResults_receiveKeyPress(int key)
{
    if (key == Qt::Key_Up)
    {
        if (ui->searchResults->currentRow() == 0)
        {
            ui->stringToBeFound->setFocus();
        }
    }
}
