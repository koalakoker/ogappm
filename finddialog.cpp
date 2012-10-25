#include "finddialog.h"
#include "ui_finddialog.h"

FindDialog::FindDialog(QWidget *parent) :
    QDialog(parent),
    ui(new Ui::FindDialog)
{
    ui->setupUi(this);
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
    int i;
    for (i = 0; i < m_resultStrList.count(); i++)
    {
        ui->searchResults->addItem(m_resultStrList.at(i));
    }
}

void FindDialog::on_searchResults_currentRowChanged(int currentRow)
{
    m_noteSelected = m_noteSelectedList.at(currentRow);
    m_numFindRequired = m_numFindRequiredList.at(currentRow);
}

void FindDialog::SetStrToBeFind(QString strToBeFind)
{
    m_strToBeFind = strToBeFind;
    ui->stringToBeFound->setText(strToBeFind);
};
