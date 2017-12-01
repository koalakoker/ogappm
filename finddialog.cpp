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
